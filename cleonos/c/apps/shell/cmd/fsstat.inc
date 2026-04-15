static int ush_cmd_fsstat(void) {
    ush_writeln("fsstat:");
    ush_print_kv_hex("  NODE_COUNT", cleonos_sys_fs_node_count());
    ush_print_kv_hex("  ROOT_CHILDREN", cleonos_sys_fs_child_count("/"));
    ush_print_kv_hex("  SYSTEM_CHILDREN", cleonos_sys_fs_child_count("/system"));
    ush_print_kv_hex("  SHELL_CHILDREN", cleonos_sys_fs_child_count("/shell"));
    ush_print_kv_hex("  TEMP_CHILDREN", cleonos_sys_fs_child_count("/temp"));
    ush_print_kv_hex("  DRIVER_CHILDREN", cleonos_sys_fs_child_count("/driver"));
    return 1;
}
