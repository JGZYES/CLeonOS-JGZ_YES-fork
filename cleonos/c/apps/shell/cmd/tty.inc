static int ush_cmd_tty(const char *arg) {
    u64 tty_count = cleonos_sys_tty_count();
    u64 active = cleonos_sys_tty_active();

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_print_kv_hex("TTY_COUNT", tty_count);
        ush_print_kv_hex("TTY_ACTIVE", active);
        return 1;
    }

    {
        u64 idx;

        if (ush_parse_u64_dec(arg, &idx) == 0) {
            ush_writeln("tty: usage tty [index]");
            return 0;
        }

        if (idx >= tty_count) {
            ush_writeln("tty: index out of range");
            return 0;
        }

        if (cleonos_sys_tty_switch(idx) == (u64)-1) {
            ush_writeln("tty: switch failed");
            return 0;
        }

        ush_writeln("tty: switched");
        ush_print_kv_hex("TTY_ACTIVE", cleonos_sys_tty_active());
        return 1;
    }
}
