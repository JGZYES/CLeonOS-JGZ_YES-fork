static int ush_cmd_pid(void) {
    ush_print_kv_hex("PID", cleonos_sys_getpid());
    return 1;
}
