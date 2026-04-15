static int ush_cmd_mkdir(const ush_state *sh, const char *arg) {
    char path[USH_PATH_MAX];

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("mkdir: directory path required");
        return 0;
    }

    if (ush_resolve_path(sh, arg, path, (u64)sizeof(path)) == 0) {
        ush_writeln("mkdir: invalid path");
        return 0;
    }

    if (ush_path_is_under_temp(path) == 0) {
        ush_writeln("mkdir: target must be under /temp");
        return 0;
    }

    if (cleonos_sys_fs_mkdir(path) == 0ULL) {
        ush_writeln("mkdir: failed");
        return 0;
    }

    return 1;
}
