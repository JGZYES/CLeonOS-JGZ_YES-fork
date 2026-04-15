static int ush_cmd_ls(const ush_state *sh, const char *arg) {
    char target[USH_PATH_MAX];
    char path[USH_PATH_MAX];
    u64 type;
    int long_mode;
    int recursive;

    if (ush_ls_parse_args(arg, &long_mode, &recursive, target, (u64)sizeof(target)) == 0) {
        ush_writeln("ls: usage ls [-l] [-R] [path]");
        return 0;
    }

    if (ush_resolve_path(sh, target, path, (u64)sizeof(path)) == 0) {
        ush_writeln("ls: invalid path");
        return 0;
    }

    type = cleonos_sys_fs_stat_type(path);

    if (type == 1ULL) {
        u64 size = cleonos_sys_fs_stat_size(path);
        ush_ls_print_one(ush_ls_basename(path), type, size, long_mode);
        return 1;
    }

    if (type != 2ULL) {
        ush_writeln("ls: path not found");
        return 0;
    }

    return ush_ls_dir(path, long_mode, recursive, recursive, 0ULL);
}
