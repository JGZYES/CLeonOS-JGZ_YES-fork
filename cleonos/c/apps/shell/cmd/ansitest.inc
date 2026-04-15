static int ush_cmd_ansitest(void) {
    u64 i;

    ush_writeln("\x1B[1;96mANSI test suite\x1B[0m");
    ush_writeln("styles: \x1B[1mbold\x1B[0m  \x1B[7minverse\x1B[0m  \x1B[4munderline\x1B[0m");
    ush_writeln("16-color demo:");
    (void)ush_cmd_ansi();

    ush_writeln("256-color palette (0..255):");
    for (i = 0ULL; i < 256ULL; i++) {
        ush_ansitest_emit_bg256(i);
        if ((i % 32ULL) == 31ULL) {
            ush_write_char('\n');
        }
    }
    ush_write_char('\n');

    ush_writeln("truecolor demo:");
    ush_writeln("  \x1B[38;2;255;64;64mRGB(255,64,64)\x1B[0m  \x1B[38;2;64;255;64mRGB(64,255,64)\x1B[0m  \x1B[38;2;64;128;255mRGB(64,128,255)\x1B[0m");

    ush_writeln("cursor control demo:");
    ush_write("  0123456789");
    ush_write("\x1B[5D");
    ush_write("\x1B[93m<OK>\x1B[0m");
    ush_write_char('\n');

    ush_write("  save");
    ush_write("\x1B[s");
    ush_write("....");
    ush_write("\x1B[u");
    ush_write("\x1B[92m<restore>\x1B[0m");
    ush_write_char('\n');

    ush_writeln("erase-line demo:");
    ush_write("  left|right-to-clear");
    ush_write("\x1B[14D\x1B[K");
    ush_write_char('\n');

    ush_writeln("ansitest done");
    return 1;
}
