static int ush_cmd_shstat(const ush_state *sh) {
    ush_writeln("shstat:");
    ush_print_kv_hex("  CMD_TOTAL", sh->cmd_total);
    ush_print_kv_hex("  CMD_OK", sh->cmd_ok);
    ush_print_kv_hex("  CMD_FAIL", sh->cmd_fail);
    ush_print_kv_hex("  CMD_UNKNOWN", sh->cmd_unknown);
    ush_print_kv_hex("  EXIT_REQUESTED", (sh->exit_requested != 0) ? 1ULL : 0ULL);
    ush_print_kv_hex("  EXIT_CODE", sh->exit_code);
    return 1;
}
