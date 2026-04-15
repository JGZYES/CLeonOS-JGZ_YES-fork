static int ush_cmd_kbdstat(void) {
    ush_writeln("kbdstat:");
    ush_print_kv_hex("  BUFFERED", cleonos_sys_kbd_buffered());
    ush_print_kv_hex("  PUSHED", cleonos_sys_kbd_pushed());
    ush_print_kv_hex("  POPPED", cleonos_sys_kbd_popped());
    ush_print_kv_hex("  DROPPED", cleonos_sys_kbd_dropped());
    ush_print_kv_hex("  HOTKEY_SWITCHES", cleonos_sys_kbd_hotkey_switches());
    return 1;
}
