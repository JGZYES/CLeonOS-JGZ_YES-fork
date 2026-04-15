static void ush_fastfetch_print_logo(int plain) {
    if (plain == 0) {
        ush_writeln("\x1B[1;34m   ______    __                         ____    _____\x1B[0m");
        ush_writeln("\x1B[1;36m  / ____/   / /   ___   ____    ____   / __ \\  / ___/\x1B[0m");
        ush_writeln("\x1B[1;32m / /       / /   / _ \\ / __ \\  / __ \\ / / / /  \\__ \\ \x1B[0m");
        ush_writeln("\x1B[1;33m/ /___    / /___/  __// /_/ / / / / // /_/ /  ___/ / \x1B[0m");
        ush_writeln("\x1B[1;31m\\____/   /_____/\\___/ \\____/ /_/ /_/ \\____/  /____/  \x1B[0m");
        ush_writeln("                                                      ");
    } else {
        ush_writeln("   ______    __                         ____    _____");
        ush_writeln("  / ____/   / /   ___   ____    ____   / __ \\  / ___/");
        ush_writeln(" / /       / /   / _ \\ / __ \\  / __ \\ / / / /  \\__ \\ ");
        ush_writeln("/ /___    / /___/  __// /_/ / / / / // /_/ /  ___/ / ");
        ush_writeln("\\____/   /_____/\\___/ \\____/ /_/ /_/ \\____/  /____/  ");
        ush_writeln("                                                      ");
    }
}
