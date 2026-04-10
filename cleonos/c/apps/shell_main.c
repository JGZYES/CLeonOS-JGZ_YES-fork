#include <cleonos_rust_bridge.h>
#include <cleonos_syscall.h>

#define SHELL_CMD_MAX 24ULL
#define SHELL_ARG_MAX 160ULL
#define SHELL_LINE_MAX 320ULL
#define SHELL_CAT_MAX 224ULL
#define SHELL_SCRIPT_MAX 1024ULL

static u64 shell_strlen(const char *str) {
    u64 len = 0ULL;

    if (str == (const char *)0) {
        return 0ULL;
    }

    while (str[len] != '\0') {
        len++;
    }

    return len;
}

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

static void shell_trim_line(char *line) {
    u64 start = 0ULL;
    u64 i = 0ULL;
    u64 len;

    if (line == (char *)0) {
        return;
    }

    while (line[start] != '\0' && shell_is_space(line[start]) != 0) {
        start++;
    }

    if (start > 0ULL) {
        while (line[start + i] != '\0') {
            line[i] = line[start + i];
            i++;
        }
        line[i] = '\0';
    }

    len = shell_strlen(line);

    while (len > 0ULL && shell_is_space(line[len - 1ULL]) != 0) {
        line[len - 1ULL] = '\0';
        len--;
    }
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
    shell_log_text("[USER][SHELL] commands: help ls <dir> cat <file> run <elf> stats");
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
        char name[CLEONOS_FS_NAME_MAX];

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

static void shell_cmd_stats(void) {
    shell_log_hex_prefixed("[USER][SHELL] fs nodes: ", cleonos_sys_fs_node_count());
    shell_log_hex_prefixed("[USER][SHELL] task count: ", cleonos_sys_task_count());
    shell_log_hex_prefixed("[USER][SHELL] service count: ", cleonos_sys_service_count());
    shell_log_hex_prefixed("[USER][SHELL] service ready: ", cleonos_sys_service_ready_count());
    shell_log_hex_prefixed("[USER][SHELL] context switches: ", cleonos_sys_context_switches());
    shell_log_hex_prefixed("[USER][SHELL] kelf count: ", cleonos_sys_kelf_count());
    shell_log_hex_prefixed("[USER][SHELL] kelf runs: ", cleonos_sys_kelf_runs());
    shell_log_hex_prefixed("[USER][SHELL] user shell ready: ", cleonos_sys_user_shell_ready());
    shell_log_hex_prefixed("[USER][SHELL] user exec requested: ", cleonos_sys_user_exec_requested());
    shell_log_hex_prefixed("[USER][SHELL] user launch tries: ", cleonos_sys_user_launch_tries());
    shell_log_hex_prefixed("[USER][SHELL] user launch ok: ", cleonos_sys_user_launch_ok());
    shell_log_hex_prefixed("[USER][SHELL] user launch fail: ", cleonos_sys_user_launch_fail());
    shell_log_hex_prefixed("[USER][SHELL] exec requests: ", cleonos_sys_exec_request_count());
    shell_log_hex_prefixed("[USER][SHELL] exec success: ", cleonos_sys_exec_success_count());
}

static void shell_execute_line(const char *line) {
    char cmd[SHELL_CMD_MAX];
    char arg[SHELL_ARG_MAX];
    char line_buf[SHELL_LINE_MAX];
    u64 i = 0ULL;

    cmd[0] = '\0';
    arg[0] = '\0';

    if (line == (const char *)0) {
        return;
    }

    while (line[i] != '\0' && i + 1ULL < (u64)sizeof(line_buf)) {
        line_buf[i] = line[i];
        i++;
    }

    line_buf[i] = '\0';
    shell_trim_line(line_buf);

    if (line_buf[0] == '\0' || line_buf[0] == '#') {
        return;
    }

    shell_log_prefixed("[USER][SHELL]$ ", line_buf);
    shell_parse_line(line_buf, cmd, (u64)sizeof(cmd), arg, (u64)sizeof(arg));

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

    if (shell_streq(cmd, "stats") != 0) {
        shell_cmd_stats();
        return;
    }

    shell_log_prefixed("[USER][SHELL] unknown command: ", cmd);
}

static void shell_run_default_script(void) {
    static const char *script[] = {
        "help",
        "stats",
        "ls /",
        "ls /system",
        "cat /README.txt",
        "run /system/elfrunner.elf",
        "run /system/memc.elf"
    };
    u64 i;

    for (i = 0ULL; i < (u64)(sizeof(script) / sizeof(script[0])); i++) {
        shell_execute_line(script[i]);
    }
}

static int shell_run_script_file(const char *path) {
    char script[SHELL_SCRIPT_MAX + 1ULL];
    char line[SHELL_LINE_MAX];
    u64 got;
    u64 i;
    u64 line_pos = 0ULL;

    if (path == (const char *)0 || path[0] == '\0') {
        return 0;
    }

    got = cleonos_sys_fs_read(path, script, SHELL_SCRIPT_MAX);

    if (got == 0ULL) {
        return 0;
    }

    if (got > SHELL_SCRIPT_MAX) {
        got = SHELL_SCRIPT_MAX;
    }

    script[got] = '\0';
    shell_log_prefixed("[USER][SHELL] script ", path);

    for (i = 0ULL; i <= got; i++) {
        char ch = script[i];

        if (ch == '\r') {
            continue;
        }

        if (ch == '\n' || ch == '\0') {
            line[line_pos] = '\0';
            shell_execute_line(line);
            line_pos = 0ULL;
            continue;
        }

        if (line_pos + 1ULL < (u64)sizeof(line)) {
            line[line_pos++] = ch;
        }
    }

    return 1;
}

int cleonos_app_main(void) {
    shell_log_text("[USER][SHELL] shell.elf command framework online");

    if (shell_run_script_file("/shell/init.cmd") == 0) {
        shell_log_text("[USER][SHELL] /shell/init.cmd missing, using default script");
        shell_run_default_script();
    }

    shell_log_text("[USER][SHELL] script done");
    return 0;
}
