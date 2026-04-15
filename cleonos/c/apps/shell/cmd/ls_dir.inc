static int ush_ls_dir(const char *path,
                      int long_mode,
                      int recursive,
                      int print_header,
                      u64 depth) {
    u64 count;
    u64 i;

    if (depth > 16ULL) {
        ush_writeln("ls: recursion depth limit reached");
        return 0;
    }

    count = cleonos_sys_fs_child_count(path);

    if (print_header != 0) {
        ush_write(path);
        ush_writeln(":");
    }

    if (count == 0ULL) {
        ush_writeln("(empty)");
    }

    for (i = 0ULL; i < count; i++) {
        char name[CLEONOS_FS_NAME_MAX];
        char child_path[USH_PATH_MAX];
        u64 type;
        u64 size = 0ULL;

        name[0] = '\0';

        if (cleonos_sys_fs_get_child_name(path, i, name) == 0ULL) {
            continue;
        }

        if (ush_ls_join_path(path, name, child_path, (u64)sizeof(child_path)) == 0) {
            continue;
        }

        type = cleonos_sys_fs_stat_type(child_path);

        if (type == 1ULL) {
            size = cleonos_sys_fs_stat_size(child_path);
        }

        ush_ls_print_one(name, type, size, long_mode);
    }

    if (recursive == 0) {
        return 1;
    }

    for (i = 0ULL; i < count; i++) {
        char name[CLEONOS_FS_NAME_MAX];
        char child_path[USH_PATH_MAX];

        name[0] = '\0';

        if (cleonos_sys_fs_get_child_name(path, i, name) == 0ULL) {
            continue;
        }

        if (ush_ls_is_dot_entry(name) != 0) {
            continue;
        }

        if (ush_ls_join_path(path, name, child_path, (u64)sizeof(child_path)) == 0) {
            continue;
        }

        if (cleonos_sys_fs_stat_type(child_path) == 2ULL) {
            ush_write_char('\n');
            (void)ush_ls_dir(child_path, long_mode, recursive, 1, depth + 1ULL);
        }
    }

    return 1;
}
