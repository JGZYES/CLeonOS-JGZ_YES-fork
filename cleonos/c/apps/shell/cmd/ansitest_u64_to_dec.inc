static u64 ush_ansitest_u64_to_dec(char *out, u64 out_size, u64 value) {
    char rev[10];
    u64 digits = 0ULL;
    u64 i;

    if (out == (char *)0 || out_size == 0ULL) {
        return 0ULL;
    }

    if (value == 0U) {
        if (out_size < 2ULL) {
            return 0ULL;
        }

        out[0] = '0';
        out[1] = '\0';
        return 1ULL;
    }

    while (value > 0U && digits < (u64)sizeof(rev)) {
        rev[digits++] = (char)('0' + (value % 10U));
        value /= 10U;
    }

    if (digits + 1ULL > out_size) {
        out[0] = '\0';
        return 0ULL;
    }

    for (i = 0ULL; i < digits; i++) {
        out[i] = rev[digits - 1ULL - i];
    }

    out[digits] = '\0';
    return digits;
}
