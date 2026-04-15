static int ush_cmd_not_supported(const char *name, const char *why) {
    ush_write(name);
    ush_write(": ");
    ush_writeln(why);
    return 0;
}
