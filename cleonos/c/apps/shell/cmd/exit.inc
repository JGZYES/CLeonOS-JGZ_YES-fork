static int ush_cmd_exit(ush_state *sh, const char *arg) {
    u64 code = 0ULL;

    if (sh == (ush_state *)0) {
        return 0;
    }

    if (arg != (const char *)0 && arg[0] != '\0') {
        if (ush_parse_u64_dec(arg, &code) == 0) {
            ush_writeln("exit: usage exit [code]");
            return 0;
        }
    }

    sh->exit_requested = 1;
    sh->exit_code = code;
    (void)cleonos_sys_exit(code);
    ush_writeln("exit: shell stopping");
    return 1;
}
