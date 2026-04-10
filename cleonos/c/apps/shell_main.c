#include <cleonos_rust_bridge.h>
#include <cleonos_syscall.h>

#define SHELL_CMD_MAX 24ULL
#define SHELL_ARG_MAX 128ULL
#define SHELL_LINE_MAX 256ULL
#define SHELL_CAT_MAX 224ULL

static int shell_streq(const char *left, const char *right) {
    u64 i = 0ULL;

    if (left == (const char *)0 || right == (const char *)0) {
        return 0;
    }

    while (left[i] != '\0' && right[i] != '\0') {
        if (left[i] != right[i]) {
            return 0;
        }
        i++;
    }

    return (left[i] == right[i]) ? 1 : 0;
}

static int shell_is_space(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') ? 1 : 0;
}

static void shell_append_char(char *dst, u64 *cursor, u64 dst_size, char ch) {
    if (*cursor + 1ULL >= dst_size) {
        return;
    }

    dst[*cursor] = ch;
    (*cursor)++;
}

static void shell_append_text(char *dst, u64 *cursor, u64 dst_size, const char *text) {
    u64 i = 0ULL;

    if (text == (const char *)0) {
        return;
    }

    while (text[i] != '\0') {
        shell_append_char(dst, cursor, dst_size, text[i]);
        i++;
    }
}

static void shell_append_hex_u64(char *dst, u64 *cursor, u64 dst_size, u64 value) {
    int nibble;

    shell_append_text(dst, cursor, dst_size, "0X");

    for (nibble = 15; nibble >= 0; nibble--) {
        u64 shift = (u64)nibble * 4ULL;
        u64 current = (value >> shift) & 0x0FULL;
        char out = (current < 10ULL) ? (char)('0' + (char)current) : (char)('A' + (char)(current - 10ULL));
        shell_append_char(dst, cursor, dst_size, out);
    }
}

static void shell_log_text(const char *text) {
    u64 len = cleonos_rust_guarded_len((const unsigned char *)text, (usize)511U);
    cleonos_sys_log_write(text, len);
}

static void shell_log_prefixed(const char *prefix, const char *value) {
    char line[SHELL_LINE_MAX];
    u64 cursor = 0ULL;

    shell_append_text(line, &cursor, (u64)sizeof(line), prefix);
    shell_append_text(line, &cursor, (u64)sizeof(line), value);
    line[cursor] = '\0';

    shell_log_text(line);
}

static void shell_log_hex_prefixed(const char *prefix, u64 value) {
    char line[SHELL_LINE_MAX];
    u64 cursor = 0ULL;

    shell_append_text(line, &cursor, (u64)sizeof(line), prefix);
    shell_append_hex_u64(line, &cursor, (u64)sizeof(line), value);
    line[cursor] = '\0';

    shell_log_text(line);
}

static void shell_parse_line(const char *line, char *out_cmd, u64 cmd_size, char *out_arg, u64 arg_size) {
    u64 i = 0ULL;
    u64 cmd_pos = 0ULL;
    u64 arg_pos = 0ULL;

    if (line == (const char *)0 || out_cmd == (char *)0 || out_arg == (char *)0) {
        return;
    }

    while (line[i] != '\0' && shell_is_space(line[i]) != 0) {
        i++;
    }

    while (line[i] != '\0' && shell_is_space(line[i]) == 0) {
        if (cmd_pos + 1ULL < cmd_size) {
            out_cmd[cmd_pos++] = line[i];
        }
        i++;
    }

    out_cmd[cmd_pos] = '\0';

    while (line[i] != '\0' && shell_is_space(line[i]) != 0) {
        i++;
    }

    while (line[i] != '\0') {
        if (arg_pos + 1ULL < arg_size) {
            out_arg[arg_pos++] = line[i];
        }
        i++;
    }

    out_arg[arg_pos] = '\0';
}

static void shell_cmd_help(void) {
    shell_log_text("[USER][SHELL] commands: help ls <dir> cat <file> run <elf>");
}

static void shell_cmd_ls(const char *arg) {
    const char *path = arg;
    u64 count;
    u64 i;

    if (path == (const char *)0 || path[0] == '\0') {
        path = "/";
    }

    count = cleonos_sys_fs_child_count(path);

    if (count == (u64)-1) {
        shell_log_text("[USER][SHELL] ls failed");
        return;
    }

    shell_log_prefixed("[USER][SHELL] ls ", path);
    shell_log_hex_prefixed("[USER][SHELL] ls count: ", count);

    for (i = 0ULL; i < count; i++) {
        char name[96];

        name[0] = '\0';

        if (cleonos_sys_fs_get_child_name(path, i, name) == 0ULL) {
            continue;
        }

        shell_log_prefixed("[USER][SHELL] - ", name);
    }
}

static void shell_cmd_cat(const char *arg) {
    char cat_buf[SHELL_CAT_MAX + 1ULL];
    u64 got;

    if (arg == (const char *)0 || arg[0] == '\0') {
        shell_log_text("[USER][SHELL] cat requires path");
        return;
    }

    got = cleonos_sys_fs_read(arg, cat_buf, SHELL_CAT_MAX);

    if (got == 0ULL) {
        shell_log_text("[USER][SHELL] cat failed");
        return;
    }

    if (got > SHELL_CAT_MAX) {
        got = SHELL_CAT_MAX;
    }

    cat_buf[got] = '\0';

    shell_log_prefixed("[USER][SHELL] cat ", arg);
    shell_log_text(cat_buf);
}

static void shell_cmd_run(const char *arg) {
    u64 status;

    if (arg == (const char *)0 || arg[0] == '\0') {
        shell_log_text("[USER][SHELL] run requires path");
        return;
    }

    status = cleonos_sys_exec_path(arg);

    if (status == 0ULL) {
        shell_log_prefixed("[USER][SHELL] run ok ", arg);
    } else {
        shell_log_prefixed("[USER][SHELL] run failed ", arg);
    }
}

static void shell_execute_line(const char *line) {
    char cmd[SHELL_CMD_MAX];
    char arg[SHELL_ARG_MAX];

    cmd[0] = '\0';
    arg[0] = '\0';

    shell_log_prefixed("[USER][SHELL]$ ", line);
    shell_parse_line(line, cmd, (u64)sizeof(cmd), arg, (u64)sizeof(arg));

    if (shell_streq(cmd, "help") != 0) {
        shell_cmd_help();
        return;
    }

    if (shell_streq(cmd, "ls") != 0) {
        shell_cmd_ls(arg);
        return;
    }

    if (shell_streq(cmd, "cat") != 0) {
        shell_cmd_cat(arg);
        return;
    }

    if (shell_streq(cmd, "run") != 0) {
        shell_cmd_run(arg);
        return;
    }

    shell_log_prefixed("[USER][SHELL] unknown command: ", cmd);
}

int cleonos_app_main(void) {
    static const char *script[] = {
        "help",
        "ls /",
        "ls /system",
        "cat /README.txt",
        "run /system/elfrunner.elf",
        "run /system/memc.elf"
    };
    u64 i;

    shell_log_text("[USER][SHELL] shell.elf command framework online");
    shell_log_hex_prefixed("[USER][SHELL] fs nodes: ", cleonos_sys_fs_node_count());
    shell_log_hex_prefixed("[USER][SHELL] task count: ", cleonos_sys_task_count());

    for (i = 0ULL; i < (u64)(sizeof(script) / sizeof(script[0])); i++) {
        shell_execute_line(script[i]);
    }

    shell_log_hex_prefixed("[USER][SHELL] exec requests: ", cleonos_sys_exec_request_count());
    shell_log_hex_prefixed("[USER][SHELL] exec success: ", cleonos_sys_exec_success_count());
    return 0;
}
