static int ush_cmd_write(const ush_state *sh, const char *arg) {
    char path_arg[USH_PATH_MAX];
    char abs_path[USH_PATH_MAX];
    const char *payload = (const char *)0;
    u64 payload_len;

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("write: usage write <file> <text>");
        return 0;
    }

    if (ush_split_first_and_rest(arg, path_arg, (u64)sizeof(path_arg), &payload) == 0) {
        ush_writeln("write: usage write <file> <text>");
        return 0;
    }

    if (ush_resolve_path(sh, path_arg, abs_path, (u64)sizeof(abs_path)) == 0) {
        ush_writeln("write: invalid path");
        return 0;
    }

    if (ush_path_is_under_temp(abs_path) == 0) {
        ush_writeln("write: target must be under /temp");
        return 0;
    }

    if (payload == (const char *)0 || payload[0] == '\0') {
        if (ush_pipeline_stdin_text == (const char *)0) {
            ush_writeln("write: usage write <file> <text>");
            return 0;
        }
        payload = ush_pipeline_stdin_text;
        payload_len = ush_pipeline_stdin_len;
    } else {
        payload_len = ush_strlen(payload);
    }

    if (cleonos_sys_fs_write(abs_path, payload, payload_len) == 0ULL) {
        ush_writeln("write: failed");
        return 0;
    }

    return 1;
}
