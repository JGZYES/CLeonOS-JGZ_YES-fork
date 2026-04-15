static int ush_cmd_exec(const ush_state *sh, const char *arg) {
    char path[USH_PATH_MAX];
    u64 status;

    if (ush_resolve_exec_path(sh, arg, path, (u64)sizeof(path)) == 0) {
        ush_writeln("exec: invalid target");
        return 0;
    }

    if (ush_path_is_under_system(path) != 0) {
        ush_writeln("exec: /system/*.elf is kernel-mode (KELF), not user-exec");
        return 0;
    }

    status = cleonos_sys_exec_path(path);

    if (status == (u64)-1) {
        ush_writeln("exec: request failed");
        return 0;
    }

    if (status == 0ULL) {
        ush_writeln("exec: request accepted");
        return 1;
    }

    ush_writeln("exec: returned non-zero status");
    return 0;
}
