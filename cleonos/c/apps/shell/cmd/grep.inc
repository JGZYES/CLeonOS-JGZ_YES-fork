static int ush_cmd_grep(const ush_state *sh, const char *arg) {
    char first[USH_PATH_MAX];
    char second[USH_PATH_MAX];
    char third[USH_PATH_MAX];
    char path[USH_PATH_MAX];
    const char *rest = "";
    const char *rest2 = "";
    const char *pattern = (const char *)0;
    const char *file_arg = (const char *)0;
    const char *input = (const char *)0;
    u64 input_len = 0ULL;
    u64 size;
    u64 got;
    int with_line_number = 0;
    static char file_buf[USH_COPY_MAX + 1U];

    if (sh == (const ush_state *)0 || arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("grep: usage grep [-n] <pattern> [file]");
        return 0;
    }

    if (ush_split_first_and_rest(arg, first, (u64)sizeof(first), &rest) == 0) {
        ush_writeln("grep: usage grep [-n] <pattern> [file]");
        return 0;
    }

    if (ush_streq(first, "-n") != 0) {
        with_line_number = 1;

        if (ush_split_first_and_rest(rest, second, (u64)sizeof(second), &rest2) == 0) {
            ush_writeln("grep: usage grep [-n] <pattern> [file]");
            return 0;
        }

        pattern = second;
        rest = rest2;
    } else {
        pattern = first;
    }

    if (rest != (const char *)0 && rest[0] != '\0') {
        if (ush_split_first_and_rest(rest, third, (u64)sizeof(third), &rest2) == 0) {
            ush_writeln("grep: usage grep [-n] <pattern> [file]");
            return 0;
        }

        file_arg = third;

        if (rest2 != (const char *)0 && rest2[0] != '\0') {
            ush_writeln("grep: usage grep [-n] <pattern> [file]");
            return 0;
        }
    }

    if (pattern == (const char *)0 || pattern[0] == '\0') {
        ush_writeln("grep: pattern required");
        return 0;
    }

    if (file_arg != (const char *)0) {
        if (ush_resolve_path(sh, file_arg, path, (u64)sizeof(path)) == 0) {
            ush_writeln("grep: invalid path");
            return 0;
        }

        if (cleonos_sys_fs_stat_type(path) != 1ULL) {
            ush_writeln("grep: file not found");
            return 0;
        }

        size = cleonos_sys_fs_stat_size(path);

        if (size == (u64)-1) {
            ush_writeln("grep: failed to stat file");
            return 0;
        }

        if (size > (u64)USH_COPY_MAX) {
            ush_writeln("grep: file too large for user buffer");
            return 0;
        }

        if (size == 0ULL) {
            return 1;
        }

        got = cleonos_sys_fs_read(path, file_buf, size);

        if (got == 0ULL || got != size) {
            ush_writeln("grep: read failed");
            return 0;
        }

        file_buf[got] = '\0';
        input = file_buf;
        input_len = got;
    } else {
        if (ush_pipeline_stdin_text == (const char *)0) {
            ush_writeln("grep: file path required (or pipeline input)");
            return 0;
        }

        input = ush_pipeline_stdin_text;
        input_len = ush_pipeline_stdin_len;
    }

    (void)ush_grep_emit_matches(input, input_len, pattern, with_line_number);
    return 1;
}
