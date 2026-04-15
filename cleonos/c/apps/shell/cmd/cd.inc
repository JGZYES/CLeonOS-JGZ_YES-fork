static int ush_cmd_cd(ush_state *sh, const char *arg) {
    const char *target = arg;
    char path[USH_PATH_MAX];

    if (target == (const char *)0 || target[0] == '\0') {
        target = "/";
    }

    if (ush_resolve_path(sh, target, path, (u64)sizeof(path)) == 0) {
        ush_writeln("cd: invalid path");
        return 0;
    }

    if (cleonos_sys_fs_stat_type(path) != 2ULL) {
        ush_writeln("cd: directory not found");
        return 0;
    }

    ush_copy(sh->cwd, (u64)sizeof(sh->cwd), path);
    return 1;
}
