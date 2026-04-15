static const char *ush_ls_basename(const char *path) {
    const char *name = path;
    u64 i = 0ULL;

    if (path == (const char *)0 || path[0] == '\0') {
        return "";
    }

    while (path[i] != '\0') {
        if (path[i] == '/' && path[i + 1ULL] != '\0') {
            name = &path[i + 1ULL];
        }

        i++;
    }

    return name;
}
