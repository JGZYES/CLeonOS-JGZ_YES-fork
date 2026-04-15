static int ush_cmd_wait(const char *arg) {
    u64 pid;
    u64 status = (u64)-1;
    u64 wait_ret;

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("wait: usage wait <pid>");
        return 0;
    }

    if (ush_parse_u64_dec(arg, &pid) == 0) {
        ush_writeln("wait: invalid pid");
        return 0;
    }

    wait_ret = cleonos_sys_wait_pid(pid, &status);

    if (wait_ret == (u64)-1) {
        ush_writeln("wait: pid not found");
        return 0;
    }

    if (wait_ret == 0ULL) {
        ush_writeln("wait: still running");
        return 1;
    }

    ush_writeln("wait: exited");
    ush_print_kv_hex("  STATUS", status);
    return 1;
}
