static int ush_ls_is_dot_entry(const char *name) {
    if (name == (const char *)0) {
        return 0;
    }

    if (name[0] == '.' && name[1] == '\0') {
        return 1;
    }

    if (name[0] == '.' && name[1] == '.' && name[2] == '\0') {
        return 1;
    }

    return 0;
}
