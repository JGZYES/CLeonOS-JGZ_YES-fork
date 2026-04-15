static int ush_cmd_fastfetch(const char *arg) {
    int plain = 0;
    u64 tty_active;
    u64 tty_count;
    u64 exec_req;
    u64 exec_ok;

    if (arg != (const char *)0 && arg[0] != '\0') {
        if (ush_streq(arg, "--plain") != 0) {
            plain = 1;
        } else if (ush_streq(arg, "--help") != 0 || ush_streq(arg, "-h") != 0) {
            ush_writeln("usage: fastfetch [--plain]");
            return 1;
        } else {
            ush_writeln("fastfetch: usage fastfetch [--plain]");
            return 0;
        }
    }

    tty_active = cleonos_sys_tty_active();
    tty_count = cleonos_sys_tty_count();
    exec_req = cleonos_sys_exec_request_count();
    exec_ok = cleonos_sys_exec_success_count();

    ush_fastfetch_print_logo(plain);
    ush_write_char('\n');

    ush_fastfetch_print_text(plain, "OS", "CLeonOS x86_64");
    ush_fastfetch_print_text(plain, "Shell", "User Shell (/shell/shell.elf)");
    ush_fastfetch_print_u64(plain, "PID", cleonos_sys_getpid());
    ush_fastfetch_print_u64(plain, "UptimeTicks", cleonos_sys_timer_ticks());
    ush_fastfetch_print_u64(plain, "Tasks", cleonos_sys_task_count());
    ush_fastfetch_print_u64(plain, "Services", cleonos_sys_service_count());
    ush_fastfetch_print_u64(plain, "SvcReady", cleonos_sys_service_ready_count());
    ush_fastfetch_print_u64(plain, "CtxSwitches", cleonos_sys_context_switches());
    ush_fastfetch_print_u64(plain, "KELFApps", cleonos_sys_kelf_count());
    ush_fastfetch_print_u64(plain, "KELFRuns", cleonos_sys_kelf_runs());
    ush_fastfetch_print_u64(plain, "FSNodes", cleonos_sys_fs_node_count());
    ush_fastfetch_print_u64(plain, "RootChildren", cleonos_sys_fs_child_count("/"));

    ush_fastfetch_write_key(plain, "TTY");
    ush_fastfetch_write_u64_dec(tty_active);
    ush_write(" / ");
    ush_fastfetch_write_u64_dec(tty_count);
    ush_write_char('\n');

    ush_fastfetch_write_key(plain, "ExecSuccess");
    ush_fastfetch_write_u64_dec(exec_ok);
    ush_write(" / ");
    ush_fastfetch_write_u64_dec(exec_req);
    ush_write_char('\n');

    ush_fastfetch_print_u64(plain, "KbdBuffered", cleonos_sys_kbd_buffered());
    ush_fastfetch_print_palette(plain);
    return 1;
}
