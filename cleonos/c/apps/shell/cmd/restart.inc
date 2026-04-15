static int ush_cmd_restart(void) {
    ush_writeln("restart: rebooting...");
    (void)cleonos_sys_restart();
    return 1;
}
