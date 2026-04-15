static int ush_grep_line_has_pattern(const char *line, u64 line_len, const char *pattern, u64 pattern_len) {
    u64 i;

    if (line == (const char *)0 || pattern == (const char *)0) {
        return 0;
    }

    if (pattern_len == 0ULL) {
        return 1;
    }

    if (pattern_len > line_len) {
        return 0;
    }

    for (i = 0ULL; i + pattern_len <= line_len; i++) {
        u64 j = 0ULL;

        while (j < pattern_len && line[i + j] == pattern[j]) {
            j++;
        }

        if (j == pattern_len) {
            return 1;
        }
    }

    return 0;
}
