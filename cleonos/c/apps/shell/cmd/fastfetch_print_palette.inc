static void ush_fastfetch_print_palette(int plain) {
    ush_fastfetch_write_key(plain, "Palette");

    if (plain != 0) {
        ush_writeln("ANSI16");
        return;
    }

    ush_write("\x1B[40m  \x1B[0m\x1B[41m  \x1B[0m\x1B[42m  \x1B[0m\x1B[43m  \x1B[0m");
    ush_write("\x1B[44m  \x1B[0m\x1B[45m  \x1B[0m\x1B[46m  \x1B[0m\x1B[47m  \x1B[0m ");
    ush_write("\x1B[100m  \x1B[0m\x1B[101m  \x1B[0m\x1B[102m  \x1B[0m\x1B[103m  \x1B[0m");
    ush_write("\x1B[104m  \x1B[0m\x1B[105m  \x1B[0m\x1B[106m  \x1B[0m\x1B[107m  \x1B[0m");
    ush_write_char('\n');
}
