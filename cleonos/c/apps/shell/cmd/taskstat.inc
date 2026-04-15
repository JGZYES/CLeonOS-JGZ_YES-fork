static int ush_cmd_taskstat(void) {
    ush_writeln("taskstat:");
    ush_print_kv_hex("  TASK_COUNT", cleonos_sys_task_count());
    ush_print_kv_hex("  CURRENT_TASK", cleonos_syscall(CLEONOS_SYSCALL_CUR_TASK, 0ULL, 0ULL, 0ULL));
    ush_print_kv_hex("  TIMER_TICKS", cleonos_sys_timer_ticks());
    ush_print_kv_hex("  CONTEXT_SWITCHES", cleonos_sys_context_switches());
    return 1;
}
