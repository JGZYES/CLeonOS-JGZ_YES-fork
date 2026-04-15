static int ush_cmd_ansi(void) {
    ush_writeln("\x1B[1;36mansi color demo\x1B[0m");
    ush_writeln("  \x1B[30mblack\x1B[0m \x1B[31mred\x1B[0m \x1B[32mgreen\x1B[0m \x1B[33myellow\x1B[0m");
    ush_writeln("  \x1B[34mblue\x1B[0m \x1B[35mmagenta\x1B[0m \x1B[36mcyan\x1B[0m \x1B[37mwhite\x1B[0m");
    ush_writeln("  \x1B[90mbright-black\x1B[0m \x1B[91mbright-red\x1B[0m \x1B[92mbright-green\x1B[0m \x1B[93mbright-yellow\x1B[0m");
    ush_writeln("  \x1B[94mbright-blue\x1B[0m \x1B[95mbright-magenta\x1B[0m \x1B[96mbright-cyan\x1B[0m \x1B[97mbright-white\x1B[0m");
    return 1;
}
