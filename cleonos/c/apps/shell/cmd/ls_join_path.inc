static int ush_ls_join_path(const char *dir_path, const char *name, char *out_path, u64 out_size) {
    u64 p = 0ULL;
    u64 i;

    if (dir_path == (const char *)0 || name == (const char *)0 || out_path == (char *)0 || out_size == 0ULL) {
        return 0;
    }

    if (dir_path[0] == '/' && dir_path[1] == '\0') {
        if (out_size < 2ULL) {
            return 0;
        }

        out_path[p++] = '/';
    } else {
        for (i = 0ULL; dir_path[i] != '\0'; i++) {
            if (p + 1ULL >= out_size) {
                return 0;
            }

            out_path[p++] = dir_path[i];
        }

        if (p == 0ULL || out_path[p - 1ULL] != '/') {
            if (p + 1ULL >= out_size) {
                return 0;
            }

            out_path[p++] = '/';
        }
    }

    for (i = 0ULL; name[i] != '\0'; i++) {
        if (p + 1ULL >= out_size) {
            return 0;
        }

        out_path[p++] = name[i];
    }

    out_path[p] = '\0';
    return 1;
}
