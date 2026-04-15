static void ush_fastfetch_print_text(int plain, const char *key, const char *value) {
    ush_fastfetch_write_key(plain, key);
    ush_writeln(value);
}
