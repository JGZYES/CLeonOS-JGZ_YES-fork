static int ush_copy_file(const char *src_path, const char *dst_path) {
    static char copy_buf[USH_COPY_MAX];
    u64 src_type;
    u64 src_size;
    u64 got;

    src_type = cleonos_sys_fs_stat_type(src_path);

    if (src_type != 1ULL) {
        ush_writeln("cp: source file not found");
        return 0;
    }

    src_size = cleonos_sys_fs_stat_size(src_path);

    if (src_size == (u64)-1) {
        ush_writeln("cp: failed to stat source");
        return 0;
    }

    if (src_size > (u64)USH_COPY_MAX) {
        ush_writeln("cp: source too large for user shell buffer");
        return 0;
    }

    if (src_size == 0ULL) {
        got = 0ULL;
    } else {
        got = cleonos_sys_fs_read(src_path, copy_buf, src_size);

        if (got == 0ULL || got != src_size) {
            ush_writeln("cp: failed to read source");
            return 0;
        }
    }

    if (cleonos_sys_fs_write(dst_path, copy_buf, got) == 0ULL) {
        ush_writeln("cp: failed to write destination");
        return 0;
    }

    return 1;
}
