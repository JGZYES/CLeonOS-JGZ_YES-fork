static void ush_fastfetch_write_u64_dec(u64 value) {
    char text[32];

    if (ush_fastfetch_u64_to_dec(text, (u64)sizeof(text), value) == 0ULL) {
        ush_write("0");
        return;
    }

    ush_write(text);
}
