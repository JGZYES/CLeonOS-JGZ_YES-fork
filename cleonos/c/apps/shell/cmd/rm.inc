static int ush_cmd_rm(const ush_state *sh, const char *arg) {
    char path[USH_PATH_MAX];

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("rm: path required");
        return 0;
    }

    if (ush_resolve_path(sh, arg, path, (u64)sizeof(path)) == 0) {
        ush_writeln("rm: invalid path");
        return 0;
    }

    if (ush_path_is_under_temp(path) == 0) {
        ush_writeln("rm: target must be under /temp");
        return 0;
    }

    if (cleonos_sys_fs_remove(path) == 0ULL) {
        ush_writeln("rm: failed (directory must be empty)");
        return 0;
    }

    return 1;
}
