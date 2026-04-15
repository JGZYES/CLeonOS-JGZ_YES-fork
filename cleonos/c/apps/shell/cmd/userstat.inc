static int ush_cmd_userstat(void) {
    ush_writeln("userstat:");
    ush_print_kv_hex("  USER_SHELL_READY", cleonos_sys_user_shell_ready());
    ush_print_kv_hex("  USER_EXEC_REQUESTED", cleonos_sys_user_exec_requested());
    ush_print_kv_hex("  USER_LAUNCH_TRIES", cleonos_sys_user_launch_tries());
    ush_print_kv_hex("  USER_LAUNCH_OK", cleonos_sys_user_launch_ok());
    ush_print_kv_hex("  USER_LAUNCH_FAIL", cleonos_sys_user_launch_fail());
    ush_print_kv_hex("  EXEC_REQUESTS", cleonos_sys_exec_request_count());
    ush_print_kv_hex("  EXEC_SUCCESS", cleonos_sys_exec_success_count());
    ush_print_kv_hex("  TTY_COUNT", cleonos_sys_tty_count());
    ush_print_kv_hex("  TTY_ACTIVE", cleonos_sys_tty_active());
    return 1;
}
