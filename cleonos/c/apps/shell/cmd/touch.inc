static int ush_cmd_touch(const ush_state *sh, const char *arg) {
    static const char empty_data[1] = {'\0'};
    char path[USH_PATH_MAX];

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("touch: file path required");
        return 0;
    }

    if (ush_resolve_path(sh, arg, path, (u64)sizeof(path)) == 0) {
        ush_writeln("touch: invalid path");
        return 0;
    }

    if (ush_path_is_under_temp(path) == 0) {
        ush_writeln("touch: target must be under /temp");
        return 0;
    }

    if (cleonos_sys_fs_write(path, empty_data, 0ULL) == 0ULL) {
        ush_writeln("touch: failed");
        return 0;
    }

    return 1;
}
