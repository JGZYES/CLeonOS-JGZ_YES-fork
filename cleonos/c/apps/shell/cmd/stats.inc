static int ush_cmd_stats(const ush_state *sh) {
    (void)ush_cmd_memstat();
    (void)ush_cmd_fsstat();
    (void)ush_cmd_taskstat();
    (void)ush_cmd_userstat();
    (void)ush_cmd_kbdstat();
    (void)ush_cmd_shstat(sh);
    return 1;
}
