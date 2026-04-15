static int ush_cmd_shutdown(void) {
    ush_writeln("shutdown: powering off...");
    (void)cleonos_sys_shutdown();
    return 1;
}
