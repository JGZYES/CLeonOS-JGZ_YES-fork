static int ush_cmd_spawn(const ush_state *sh, const char *arg) {
    char path[USH_PATH_MAX];
    u64 pid;

    if (ush_resolve_exec_path(sh, arg, path, (u64)sizeof(path)) == 0) {
        ush_writeln("spawn: invalid target");
        return 0;
    }

    if (ush_path_is_under_system(path) != 0) {
        ush_writeln("spawn: /system/*.elf is kernel-mode (KELF), not user-exec");
        return 0;
    }

    pid = cleonos_sys_spawn_path(path);

    if (pid == (u64)-1) {
        ush_writeln("spawn: request failed");
        return 0;
    }

    ush_writeln("spawn: completed");
    ush_print_kv_hex("  PID", pid);
    return 1;
}
