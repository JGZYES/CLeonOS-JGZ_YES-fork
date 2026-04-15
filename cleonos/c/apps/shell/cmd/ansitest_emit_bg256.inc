static void ush_ansitest_emit_bg256(u64 index) {
    char num[4];
    char seq[24];
    u64 digits;
    u64 p = 0ULL;
    u64 i;

    if (index > 255U) {
        index = 255U;
    }

    digits = ush_ansitest_u64_to_dec(num, (u64)sizeof(num), index);
    if (digits == 0ULL) {
        return;
    }

    seq[p++] = '\x1B';
    seq[p++] = '[';
    seq[p++] = '4';
    seq[p++] = '8';
    seq[p++] = ';';
    seq[p++] = '5';
    seq[p++] = ';';

    for (i = 0ULL; i < digits && p + 1ULL < (u64)sizeof(seq); i++) {
        seq[p++] = num[i];
    }

    if (p + 7ULL >= (u64)sizeof(seq)) {
        return;
    }

    seq[p++] = 'm';
    seq[p++] = ' ';
    seq[p++] = ' ';
    seq[p++] = '\x1B';
    seq[p++] = '[';
    seq[p++] = '0';
    seq[p++] = 'm';
    seq[p] = '\0';

    ush_write(seq);
}
