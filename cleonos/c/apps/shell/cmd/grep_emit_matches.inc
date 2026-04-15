static u64 ush_grep_emit_matches(const char *input, u64 input_len, const char *pattern, int with_line_number) {
    u64 matches = 0ULL;
    u64 line_no = 1ULL;
    u64 start = 0ULL;
    u64 i;
    u64 pattern_len;

    if (input == (const char *)0 || pattern == (const char *)0) {
        return 0ULL;
    }

    pattern_len = ush_strlen(pattern);

    for (i = 0ULL; i <= input_len; i++) {
        if (i == input_len || input[i] == '\n') {
            u64 line_len = i - start;

            if (ush_grep_line_has_pattern(&input[start], line_len, pattern, pattern_len) != 0) {
                u64 j;

                matches++;

                if (with_line_number != 0) {
                    ush_grep_write_u64_dec(line_no);
                    ush_write(":");
                }

                for (j = 0ULL; j < line_len; j++) {
                    ush_write_char(input[start + j]);
                }

                ush_write_char('\n');
            }

            start = i + 1ULL;
            line_no++;
        }
    }

    return matches;
}
