static int ush_cmd_memstat(void) {
    ush_writeln("memstat (user ABI limited):");
    ush_print_kv_hex("  SERVICE_COUNT", cleonos_sys_service_count());
    ush_print_kv_hex("  SERVICE_READY_COUNT", cleonos_sys_service_ready_count());
    ush_print_kv_hex("  KELF_COUNT", cleonos_sys_kelf_count());
    ush_print_kv_hex("  KELF_RUNS", cleonos_sys_kelf_runs());
    return 1;
}
