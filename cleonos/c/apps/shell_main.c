#include <cleonos_syscall.h>

#define SHELL_CMD_MAX      32ULL
#define SHELL_ARG_MAX      160ULL
#define SHELL_LINE_MAX     192ULL
#define SHELL_CAT_MAX      512ULL
#define SHELL_SCRIPT_MAX   1024ULL
#define SHELL_CLEAR_LINES   56ULL

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

static int shell_is_printable(char ch) {
    return (ch >= 32 && ch <= 126) ? 1 : 0;
}

static void shell_copy(char *dst, u64 dst_size, const char *src) {
    u64 i = 0ULL;

    if (dst == (char *)0 || src == (const char *)0 || dst_size == 0ULL) {
        return;
    }

    while (src[i] != '\0' && i + 1ULL < dst_size) {
        dst[i] = src[i];
        i++;
    }

    dst[i] = '\0';
}

static void shell_write(const char *text) {
    u64 len;

    if (text == (const char *)0) {
        return;
    }

    len = shell_strlen(text);

    if (len == 0ULL) {
        return;
    }

    (void)cleonos_sys_tty_write(text, len);
}

static void shell_write_char(char ch) {
    (void)cleonos_sys_tty_write_char(ch);
}

static void shell_writeln(const char *text) {
    shell_write(text);
    shell_write_char('\n');
}

static void shell_prompt(void) {
    shell_write("cleonos(user)> ");
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

    out_cmd[0] = '\0';
    out_arg[0] = '\0';

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

static int shell_has_suffix(const char *name, const char *suffix) {
    u64 name_len;
    u64 suffix_len;
    u64 i;

    if (name == (const char *)0 || suffix == (const char *)0) {
        return 0;
    }

    name_len = shell_strlen(name);
    suffix_len = shell_strlen(suffix);

    if (suffix_len > name_len) {
        return 0;
    }

    for (i = 0ULL; i < suffix_len; i++) {
        if (name[name_len - suffix_len + i] != suffix[i]) {
            return 0;
        }
    }

    return 1;
}

static int shell_resolve_exec_path(const char *arg, char *out_path, u64 out_size) {
    u64 cursor = 0ULL;
    u64 i;

    if (arg == (const char *)0 || out_path == (char *)0 || out_size == 0ULL) {
        return 0;
    }

    if (arg[0] == '\0') {
        return 0;
    }

    out_path[0] = '\0';

    if (arg[0] == '/') {
        shell_copy(out_path, out_size, arg);
        return 1;
    }

    {
        static const char prefix[] = "/shell/";
        u64 prefix_len = (u64)(sizeof(prefix) - 1U);

        if (prefix_len + 1ULL >= out_size) {
            return 0;
        }

        for (i = 0ULL; i < prefix_len; i++) {
            out_path[cursor++] = prefix[i];
        }
    }

    i = 0ULL;
    while (arg[i] != '\0' && cursor + 1ULL < out_size) {
        out_path[cursor++] = arg[i++];
    }

    out_path[cursor] = '\0';

    if (arg[i] != '\0') {
        return 0;
    }

    if (shell_has_suffix(out_path, ".elf") == 0) {
        static const char suffix[] = ".elf";

        for (i = 0ULL; suffix[i] != '\0'; i++) {
            if (cursor + 1ULL >= out_size) {
                return 0;
            }
            out_path[cursor++] = suffix[i];
        }

        out_path[cursor] = '\0';
    }

    return 1;
}

static void shell_cmd_help(void) {
    shell_writeln("commands:");
    shell_writeln("  help");
    shell_writeln("  ls [dir]");
    shell_writeln("  cat <file>");
    shell_writeln("  exec <path|name>");
    shell_writeln("  clear");
    shell_writeln("  stats");
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
        shell_writeln("ls: directory not found");
        return;
    }

    if (count == 0ULL) {
        shell_writeln("(empty)");
        return;
    }

    for (i = 0ULL; i < count; i++) {
        char name[CLEONOS_FS_NAME_MAX];

        name[0] = '\0';

        if (cleonos_sys_fs_get_child_name(path, i, name) == 0ULL) {
            continue;
        }

        shell_writeln(name);
    }
}

static void shell_cmd_cat(const char *arg) {
    char cat_buf[SHELL_CAT_MAX + 1ULL];
    u64 got;

    if (arg == (const char *)0 || arg[0] == '\0') {
        shell_writeln("cat: file path required");
        return;
    }

    got = cleonos_sys_fs_read(arg, cat_buf, SHELL_CAT_MAX);

    if (got == 0ULL) {
        shell_writeln("cat: file not found");
        return;
    }

    if (got > SHELL_CAT_MAX) {
        got = SHELL_CAT_MAX;
    }

    cat_buf[got] = '\0';
    shell_writeln(cat_buf);
}

static void shell_cmd_exec(const char *arg) {
    char path[SHELL_LINE_MAX];
    u64 status;

    if (shell_resolve_exec_path(arg, path, (u64)sizeof(path)) == 0) {
        shell_writeln("exec: invalid target");
        return;
    }

    status = cleonos_sys_exec_path(path);

    if (status == 0ULL) {
        shell_writeln("exec: request accepted");
    } else {
        shell_writeln("exec: request failed");
    }
}

static void shell_cmd_clear(void) {
    u64 i;

    for (i = 0ULL; i < SHELL_CLEAR_LINES; i++) {
        shell_write_char('\n');
    }
}

static void shell_cmd_stats(void) {
    (void)cleonos_sys_task_count();
    shell_writeln("stats: use kernel log channel for full counters");
}

static void shell_execute_line(const char *line) {
    char cmd[SHELL_CMD_MAX];
    char arg[SHELL_ARG_MAX];
    char line_buf[SHELL_LINE_MAX];
    u64 i = 0ULL;

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

    if (shell_streq(cmd, "exec") != 0 || shell_streq(cmd, "run") != 0) {
        shell_cmd_exec(arg);
        return;
    }

    if (shell_streq(cmd, "clear") != 0) {
        shell_cmd_clear();
        return;
    }

    if (shell_streq(cmd, "stats") != 0) {
        shell_cmd_stats();
        return;
    }

    shell_writeln("unknown command; type 'help'");
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

static char shell_read_char_blocking(void) {
    for (;;) {
        u64 ch = cleonos_sys_kbd_get_char();

        if (ch != (u64)-1) {
            return (char)(ch & 0xFFULL);
        }

        __asm__ volatile("pause");
    }
}

static void shell_read_line(char *out_line, u64 out_size) {
    u64 cursor = 0ULL;

    if (out_line == (char *)0 || out_size == 0ULL) {
        return;
    }

    out_line[0] = '\0';

    for (;;) {
        char ch = shell_read_char_blocking();

        if (ch == '\r') {
            continue;
        }

        if (ch == '\n') {
            shell_write_char('\n');
            break;
        }

        if (ch == '\b' || ch == 127) {
            if (cursor > 0ULL) {
                cursor--;
                out_line[cursor] = '\0';
                shell_write_char('\b');
            }
            continue;
        }

        if (ch == '\t') {
            ch = ' ';
        }

        if (shell_is_printable(ch) == 0) {
            continue;
        }

        if (cursor + 1ULL >= out_size) {
            continue;
        }

        out_line[cursor++] = ch;
        out_line[cursor] = '\0';
        shell_write_char(ch);
    }
}

int cleonos_app_main(void) {
    char line[SHELL_LINE_MAX];

    shell_writeln("[USER][SHELL] interactive framework online");

    if (shell_run_script_file("/shell/init.cmd") == 0) {
        shell_writeln("[USER][SHELL] /shell/init.cmd missing");
    }

    for (;;) {
        shell_prompt();
        shell_read_line(line, (u64)sizeof(line));
        shell_execute_line(line);
    }
}