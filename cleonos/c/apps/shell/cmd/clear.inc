static int ush_cmd_clear(void) {
    ush_write("\x1B[2J\x1B[H");
    return 1;
}
