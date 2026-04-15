static void ush_fastfetch_write_key(int plain, const char *key) {
    ush_write("  ");
    if (plain == 0) {
        ush_write("\x1B[1;96m");
    }
    ush_write(key);
    if (plain == 0) {
        ush_write("\x1B[0m");
    }
    ush_write(": ");
}
