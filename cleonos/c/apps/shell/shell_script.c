#include "shell_internal.h"

int ush_run_script_file(ush_state *sh, const char *path) {
    char script[USH_SCRIPT_MAX + 1ULL];
    char line[USH_LINE_MAX];
    u64 got;
    u64 i;
    u64 line_pos = 0ULL;

    if (sh == (ush_state *)0 || path == (const char *)0 || path[0] == '\0') {
        return 0;
    }

    got = cleonos_sys_fs_read(path, script, USH_SCRIPT_MAX);

    if (got == 0ULL) {
        return 0;
    }

    if (got > USH_SCRIPT_MAX) {
        got = USH_SCRIPT_MAX;
    }

    script[got] = '\0';

    for (i = 0ULL; i <= got; i++) {
        char ch = script[i];

        if (ch == '\r') {
            continue;
        }

        if (ch == '\n' || ch == '\0') {
            line[line_pos] = '\0';
            ush_execute_line(sh, line);
            line_pos = 0ULL;
            continue;
        }

        if (line_pos + 1ULL < (u64)sizeof(line)) {
            line[line_pos++] = ch;
        }
    }

    return 1;
}
