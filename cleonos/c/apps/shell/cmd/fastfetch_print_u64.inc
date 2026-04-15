static void ush_fastfetch_print_u64(int plain, const char *key, u64 value) {
    ush_fastfetch_write_key(plain, key);
    ush_fastfetch_write_u64_dec(value);
    ush_write_char('\n');
}
