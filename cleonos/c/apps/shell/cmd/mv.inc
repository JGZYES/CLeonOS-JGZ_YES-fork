static int ush_cmd_mv(const ush_state *sh, const char *arg) {
    char src_arg[USH_PATH_MAX];
    char dst_arg[USH_PATH_MAX];
    char src_path[USH_PATH_MAX];
    char dst_path[USH_PATH_MAX];

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("mv: usage mv <src> <dst>");
        return 0;
    }

    if (ush_split_two_args(arg, src_arg, (u64)sizeof(src_arg), dst_arg, (u64)sizeof(dst_arg)) == 0) {
        ush_writeln("mv: usage mv <src> <dst>");
        return 0;
    }

    if (ush_resolve_path(sh, src_arg, src_path, (u64)sizeof(src_path)) == 0 ||
        ush_resolve_path(sh, dst_arg, dst_path, (u64)sizeof(dst_path)) == 0) {
        ush_writeln("mv: invalid path");
        return 0;
    }

    if (ush_path_is_under_temp(src_path) == 0 || ush_path_is_under_temp(dst_path) == 0) {
        ush_writeln("mv: source and destination must be under /temp");
        return 0;
    }

    if (ush_copy_file(src_path, dst_path) == 0) {
        return 0;
    }

    if (cleonos_sys_fs_remove(src_path) == 0ULL) {
        ush_writeln("mv: source remove failed");
        return 0;
    }

    return 1;
}
