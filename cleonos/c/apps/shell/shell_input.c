#include "shell_internal.h"

static void ush_history_cancel_nav(ush_state *sh) {
    if (sh == (ush_state *)0) {
        return;
    }

    sh->history_nav = -1;
    sh->nav_saved_len = 0ULL;
    sh->nav_saved_cursor = 0ULL;
    sh->nav_saved_line[0] = '\0';
}

static void ush_reset_line(ush_state *sh) {
    if (sh == (ush_state *)0) {
        return;
    }

    sh->line_len = 0ULL;
    sh->cursor = 0ULL;
    sh->rendered_len = 0ULL;
    sh->line[0] = '\0';
}

static void ush_load_line(ush_state *sh, const char *line) {
    if (sh == (ush_state *)0) {
        return;
    }

    if (line == (const char *)0) {
        ush_reset_line(sh);
        return;
    }

    ush_copy(sh->line, (u64)sizeof(sh->line), line);
    sh->line_len = ush_strlen(sh->line);
    sh->cursor = sh->line_len;
}

static void ush_render_line(ush_state *sh) {
    u64 i;

    if (sh == (ush_state *)0) {
        return;
    }

    ush_write_char('\r');
    ush_prompt(sh);

    for (i = 0ULL; i < sh->line_len; i++) {
        ush_write_char(sh->line[i]);
    }

    for (i = sh->line_len; i < sh->rendered_len; i++) {
        ush_write_char(' ');
    }

    ush_write_char('\r');
    ush_prompt(sh);

    for (i = 0ULL; i < sh->cursor; i++) {
        ush_write_char(sh->line[i]);
    }

    sh->rendered_len = sh->line_len;
}

static int ush_line_has_non_space(const char *line) {
    u64 i = 0ULL;

    if (line == (const char *)0) {
        return 0;
    }

    while (line[i] != '\0') {
        if (ush_is_space(line[i]) == 0) {
            return 1;
        }
        i++;
    }

    return 0;
}

static void ush_history_push(ush_state *sh, const char *line) {
    if (sh == (ush_state *)0) {
        return;
    }

    if (ush_line_has_non_space(line) == 0) {
        ush_history_cancel_nav(sh);
        return;
    }

    if (sh->history_count > 0ULL && ush_streq(sh->history[sh->history_count - 1ULL], line) != 0) {
        ush_history_cancel_nav(sh);
        return;
    }

    if (sh->history_count < USH_HISTORY_MAX) {
        ush_copy(sh->history[sh->history_count], (u64)sizeof(sh->history[sh->history_count]), line);
        sh->history_count++;
    } else {
        u64 i;

        for (i = 1ULL; i < USH_HISTORY_MAX; i++) {
            ush_copy(sh->history[i - 1ULL], (u64)sizeof(sh->history[i - 1ULL]), sh->history[i]);
        }

        ush_copy(sh->history[USH_HISTORY_MAX - 1ULL], (u64)sizeof(sh->history[USH_HISTORY_MAX - 1ULL]), line);
    }

    ush_history_cancel_nav(sh);
}

static void ush_history_apply_current(ush_state *sh) {
    if (sh == (ush_state *)0) {
        return;
    }

    if (sh->history_nav >= 0) {
        ush_load_line(sh, sh->history[(u64)sh->history_nav]);
    } else {
        ush_copy(sh->line, (u64)sizeof(sh->line), sh->nav_saved_line);
        sh->line_len = sh->nav_saved_len;
        if (sh->line_len > USH_LINE_MAX - 1ULL) {
            sh->line_len = USH_LINE_MAX - 1ULL;
            sh->line[sh->line_len] = '\0';
        }
        sh->cursor = sh->nav_saved_cursor;
        if (sh->cursor > sh->line_len) {
            sh->cursor = sh->line_len;
        }
    }

    ush_render_line(sh);
}

static void ush_history_up(ush_state *sh) {
    if (sh == (ush_state *)0 || sh->history_count == 0ULL) {
        return;
    }

    if (sh->history_nav < 0) {
        ush_copy(sh->nav_saved_line, (u64)sizeof(sh->nav_saved_line), sh->line);
        sh->nav_saved_len = sh->line_len;
        sh->nav_saved_cursor = sh->cursor;
        sh->history_nav = (i64)sh->history_count - 1;
    } else if (sh->history_nav > 0) {
        sh->history_nav--;
    }

    ush_history_apply_current(sh);
}

static void ush_history_down(ush_state *sh) {
    if (sh == (ush_state *)0 || sh->history_nav < 0) {
        return;
    }

    if ((u64)sh->history_nav + 1ULL < sh->history_count) {
        sh->history_nav++;
    } else {
        sh->history_nav = -1;
    }

    ush_history_apply_current(sh);
}

static char ush_read_char_blocking(void) {
    for (;;) {
        u64 ch = cleonos_sys_kbd_get_char();

        if (ch != (u64)-1) {
            return (char)(ch & 0xFFULL);
        }

        __asm__ volatile("pause");
    }
}

void ush_read_line(ush_state *sh, char *out_line, u64 out_size) {
    if (sh == (ush_state *)0 || out_line == (char *)0 || out_size == 0ULL) {
        return;
    }

    ush_reset_line(sh);
    ush_history_cancel_nav(sh);

    out_line[0] = '\0';

    ush_prompt(sh);

    for (;;) {
        char ch = ush_read_char_blocking();

        if (ch == '\r') {
            continue;
        }

        if (ch == '\n') {
            ush_write_char('\n');
            sh->line[sh->line_len] = '\0';
            ush_history_push(sh, sh->line);
            ush_copy(out_line, out_size, sh->line);
            ush_reset_line(sh);
            return;
        }

        if (ch == USH_KEY_UP) {
            ush_history_up(sh);
            continue;
        }

        if (ch == USH_KEY_DOWN) {
            ush_history_down(sh);
            continue;
        }

        if (ch == USH_KEY_LEFT) {
            if (sh->cursor > 0ULL) {
                sh->cursor--;
                ush_render_line(sh);
            }
            continue;
        }

        if (ch == USH_KEY_RIGHT) {
            if (sh->cursor < sh->line_len) {
                sh->cursor++;
                ush_render_line(sh);
            }
            continue;
        }

        if (ch == USH_KEY_HOME) {
            if (sh->cursor != 0ULL) {
                sh->cursor = 0ULL;
                ush_render_line(sh);
            }
            continue;
        }

        if (ch == USH_KEY_END) {
            if (sh->cursor != sh->line_len) {
                sh->cursor = sh->line_len;
                ush_render_line(sh);
            }
            continue;
        }

        if (ch == '\b' || ch == 127) {
            if (sh->cursor > 0ULL && sh->line_len > 0ULL) {
                u64 i;

                ush_history_cancel_nav(sh);

                for (i = sh->cursor - 1ULL; i < sh->line_len; i++) {
                    sh->line[i] = sh->line[i + 1ULL];
                }

                sh->line_len--;
                sh->cursor--;
                ush_render_line(sh);
            }
            continue;
        }

        if (ch == USH_KEY_DELETE) {
            if (sh->cursor < sh->line_len) {
                u64 i;

                ush_history_cancel_nav(sh);

                for (i = sh->cursor; i < sh->line_len; i++) {
                    sh->line[i] = sh->line[i + 1ULL];
                }

                sh->line_len--;
                ush_render_line(sh);
            }
            continue;
        }

        if (ch == '\t') {
            ch = ' ';
        }

        if (ush_is_printable(ch) == 0) {
            continue;
        }

        if (sh->line_len + 1ULL >= USH_LINE_MAX) {
            continue;
        }

        ush_history_cancel_nav(sh);

        if (sh->cursor == sh->line_len) {
            sh->line[sh->line_len++] = ch;
            sh->line[sh->line_len] = '\0';
            sh->cursor = sh->line_len;
            ush_write_char(ch);
            sh->rendered_len = sh->line_len;
            continue;
        }

        {
            u64 i;

            for (i = sh->line_len; i > sh->cursor; i--) {
                sh->line[i] = sh->line[i - 1ULL];
            }

            sh->line[sh->cursor] = ch;
            sh->line_len++;
            sh->cursor++;
            sh->line[sh->line_len] = '\0';
            ush_render_line(sh);
        }
    }
}
