static int ush_ls_parse_args(const char *arg,
                             int *out_long_mode,
                             int *out_recursive,
                             char *out_target,
                             u64 out_target_size) {
    char token[USH_PATH_MAX];
    u64 i = 0ULL;
    int path_set = 0;

    if (out_long_mode == (int *)0 ||
        out_recursive == (int *)0 ||
        out_target == (char *)0 ||
        out_target_size == 0ULL) {
        return 0;
    }

    *out_long_mode = 0;
    *out_recursive = 0;
    ush_copy(out_target, out_target_size, ".");

    if (arg == (const char *)0 || arg[0] == '\0') {
        return 1;
    }

    while (arg[i] != '\0') {
        u64 p = 0ULL;
        u64 j;

        while (arg[i] != '\0' && ush_is_space(arg[i]) != 0) {
            i++;
        }

        if (arg[i] == '\0') {
            break;
        }

        while (arg[i] != '\0' && ush_is_space(arg[i]) == 0) {
            if (p + 1ULL < (u64)sizeof(token)) {
                token[p++] = arg[i];
            }

            i++;
        }

        token[p] = '\0';

        if (token[0] == '-' && token[1] != '\0') {
            for (j = 1ULL; token[j] != '\0'; j++) {
                if (token[j] == 'l') {
                    *out_long_mode = 1;
                } else if (token[j] == 'R') {
                    *out_recursive = 1;
                } else {
                    return 0;
                }
            }

            continue;
        }

        if (path_set != 0) {
            return 0;
        }

        ush_copy(out_target, out_target_size, token);
        path_set = 1;
    }

    return 1;
}
