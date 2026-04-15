static int ush_cmd_sleep(const char *arg) {
    u64 ticks;
    u64 elapsed;

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("sleep: usage sleep <ticks>");
        return 0;
    }

    if (ush_parse_u64_dec(arg, &ticks) == 0) {
        ush_writeln("sleep: invalid ticks");
        return 0;
    }

    elapsed = cleonos_sys_sleep_ticks(ticks);
    ush_print_kv_hex("SLEPT_TICKS", elapsed);
    return 1;
}
