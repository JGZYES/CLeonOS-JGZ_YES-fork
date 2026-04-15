static int ush_cmd_dmesg(const char *arg) {
    u64 total = cleonos_sys_log_journal_count();
    u64 limit = USH_DMESG_DEFAULT;
    u64 start;
    u64 i;

    if (arg != (const char *)0 && arg[0] != '\0') {
        if (ush_parse_u64_dec(arg, &limit) == 0 || limit == 0ULL) {
            ush_writeln("dmesg: usage dmesg [positive_count]");
            return 0;
        }
    }

    if (total == 0ULL) {
        ush_writeln("(journal empty)");
        return 1;
    }

    if (limit > total) {
        limit = total;
    }

    start = total - limit;

    for (i = start; i < total; i++) {
        char line[USH_DMESG_LINE_MAX];

        if (cleonos_sys_log_journal_read(i, line, (u64)sizeof(line)) != 0ULL) {
            ush_writeln(line);
        }
    }

    return 1;
}
