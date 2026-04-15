static void ush_ls_print_one(const char *name, u64 type, u64 size, int long_mode) {
    if (long_mode == 0) {
        ush_writeln(name);
        return;
    }

    if (type == 2ULL) {
        ush_write("d ");
    } else if (type == 1ULL) {
        ush_write("f ");
    } else {
        ush_write("? ");
    }

    ush_write(name);

    if (type == 1ULL) {
        ush_write("  size=");
        ush_write_hex_u64(size);
    } else if (type == 2ULL) {
        ush_write("  <DIR>");
    } else {
        ush_write("  <UNKNOWN>");
    }

    ush_write_char('\n');
}
