static int ush_cmd_yield(void) {
    ush_print_kv_hex("YIELD_TICK", cleonos_sys_yield());
    return 1;
}
