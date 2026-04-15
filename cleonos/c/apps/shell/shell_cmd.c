#include "shell_internal.h"

#define USH_DMESG_DEFAULT   64ULL
#define USH_DMESG_LINE_MAX 256ULL
#define USH_COPY_MAX      65536U
#define USH_PIPELINE_MAX_STAGES 8ULL
#define USH_PIPE_CAPTURE_MAX   USH_COPY_MAX

typedef struct ush_pipeline_stage {
    char text[USH_LINE_MAX];
    char cmd[USH_CMD_MAX];
    char arg[USH_ARG_MAX];
    char redirect_path[USH_PATH_MAX];
    int redirect_mode; /* 0: none, 1: >, 2: >> */
} ush_pipeline_stage;

static const char *ush_pipeline_stdin_text = (const char *)0;
static u64 ush_pipeline_stdin_len = 0ULL;
static char ush_pipeline_capture_a[USH_PIPE_CAPTURE_MAX + 1U];
static char ush_pipeline_capture_b[USH_PIPE_CAPTURE_MAX + 1U];


static int ush_path_is_under_temp(const char *path) {
    if (path == (const char *)0) {
        return 0;
    }

    if (path[0] != '/' || path[1] != 't' || path[2] != 'e' || path[3] != 'm' || path[4] != 'p') {
        return 0;
    }

    return (path[5] == '\0' || path[5] == '/') ? 1 : 0;
}

static int ush_split_first_and_rest(const char *arg, char *out_first, u64 out_first_size, const char **out_rest) {
    u64 i = 0ULL;
    u64 p = 0ULL;

    if (arg == (const char *)0 || out_first == (char *)0 || out_first_size == 0ULL || out_rest == (const char **)0) {
        return 0;
    }

    out_first[0] = '\0';
    *out_rest = "";

    while (arg[i] != '\0' && ush_is_space(arg[i]) != 0) {
        i++;
    }

    if (arg[i] == '\0') {
        return 0;
    }

    while (arg[i] != '\0' && ush_is_space(arg[i]) == 0) {
        if (p + 1ULL < out_first_size) {
            out_first[p++] = arg[i];
        }
        i++;
    }

    out_first[p] = '\0';

    while (arg[i] != '\0' && ush_is_space(arg[i]) != 0) {
        i++;
    }

    *out_rest = &arg[i];
    return 1;
}

static int ush_split_two_args(const char *arg,
                              char *out_first,
                              u64 out_first_size,
                              char *out_second,
                              u64 out_second_size) {
    u64 i = 0ULL;
    u64 p = 0ULL;

    if (arg == (const char *)0 ||
        out_first == (char *)0 || out_first_size == 0ULL ||
        out_second == (char *)0 || out_second_size == 0ULL) {
        return 0;
    }

    out_first[0] = '\0';
    out_second[0] = '\0';

    while (arg[i] != '\0' && ush_is_space(arg[i]) != 0) {
        i++;
    }

    if (arg[i] == '\0') {
        return 0;
    }

    while (arg[i] != '\0' && ush_is_space(arg[i]) == 0) {
        if (p + 1ULL < out_first_size) {
            out_first[p++] = arg[i];
        }
        i++;
    }

    out_first[p] = '\0';

    while (arg[i] != '\0' && ush_is_space(arg[i]) != 0) {
        i++;
    }

    if (arg[i] == '\0') {
        return 0;
    }

    p = 0ULL;
    while (arg[i] != '\0' && ush_is_space(arg[i]) == 0) {
        if (p + 1ULL < out_second_size) {
            out_second[p++] = arg[i];
        }
        i++;
    }

    out_second[p] = '\0';

    return (out_first[0] != '\0' && out_second[0] != '\0') ? 1 : 0;
}

#include "cmd/help.inc"

#include "cmd/ls_join_path.inc"

#include "cmd/ls_basename.inc"

#include "cmd/ls_is_dot_entry.inc"

#include "cmd/ls_print_one.inc"

#include "cmd/ls_parse_args.inc"

#include "cmd/ls_dir.inc"

#include "cmd/ls.inc"

#include "cmd/cat.inc"

#include "cmd/grep_write_u64_dec.inc"

#include "cmd/grep_line_has_pattern.inc"

#include "cmd/grep_emit_matches.inc"

#include "cmd/grep.inc"
#include "cmd/pwd.inc"

#include "cmd/cd.inc"

#include "cmd/exec.inc"

#include "cmd/pid.inc"

#include "cmd/spawn.inc"

#include "cmd/wait.inc"

#include "cmd/sleep.inc"

#include "cmd/yield.inc"


#include "cmd/shutdown.inc"

#include "cmd/restart.inc"

#include "cmd/exit.inc"

#include "cmd/clear.inc"

#include "cmd/ansi.inc"

#include "cmd/ansitest_u64_to_dec.inc"

#include "cmd/ansitest_emit_bg256.inc"

#include "cmd/ansitest.inc"
#include "cmd/fastfetch_u64_to_dec.inc"

#include "cmd/fastfetch_write_u64_dec.inc"

#include "cmd/fastfetch_write_key.inc"

#include "cmd/fastfetch_print_text.inc"

#include "cmd/fastfetch_print_u64.inc"

#include "cmd/fastfetch_print_logo.inc"

#include "cmd/fastfetch_print_palette.inc"

#include "cmd/fastfetch.inc"
#include "cmd/kbdstat.inc"

#include "cmd/memstat.inc"

#include "cmd/fsstat.inc"

#include "cmd/taskstat.inc"

#include "cmd/userstat.inc"

#include "cmd/shstat.inc"

#include "cmd/tty.inc"

#include "cmd/dmesg.inc"

#include "cmd/mkdir.inc"

#include "cmd/touch.inc"

#include "cmd/write.inc"

#include "cmd/append.inc"

#include "cmd/copy_file.inc"

#include "cmd/cp.inc"

#include "cmd/mv.inc"

#include "cmd/rm.inc"

#include "cmd/stats.inc"

#include "cmd/not_supported.inc"

static int ush_execute_single_command(ush_state *sh,
                                      const char *cmd,
                                      const char *arg,
                                      int allow_external,
                                      int *out_known,
                                      int *out_success) {
    int known = 1;
    int success = 0;

    if (out_known != (int *)0) {
        *out_known = 1;
    }

    if (out_success != (int *)0) {
        *out_success = 0;
    }

    if (sh == (ush_state *)0 || cmd == (const char *)0 || cmd[0] == '\0') {
        if (out_known != (int *)0) {
            *out_known = 0;
        }
        return 0;
    }

    if (allow_external != 0 && ush_try_exec_external(sh, cmd, arg, &success) != 0) {
        if (out_success != (int *)0) {
            *out_success = success;
        }
        return 1;
    }

    if (ush_streq(cmd, "help") != 0) {
        success = ush_cmd_help();
    } else if (ush_streq(cmd, "ls") != 0 || ush_streq(cmd, "dir") != 0) {
        success = ush_cmd_ls(sh, arg);
    } else if (ush_streq(cmd, "cat") != 0) {
        success = ush_cmd_cat(sh, arg);
    } else if (ush_streq(cmd, "grep") != 0) {
        success = ush_cmd_grep(sh, arg);
    } else if (ush_streq(cmd, "pwd") != 0) {
        success = ush_cmd_pwd(sh);
    } else if (ush_streq(cmd, "cd") != 0) {
        success = ush_cmd_cd(sh, arg);
    } else if (ush_streq(cmd, "exec") != 0 || ush_streq(cmd, "run") != 0) {
        success = ush_cmd_exec(sh, arg);
    } else if (ush_streq(cmd, "pid") != 0) {
        success = ush_cmd_pid();
    } else if (ush_streq(cmd, "spawn") != 0) {
        success = ush_cmd_spawn(sh, arg);
    } else if (ush_streq(cmd, "wait") != 0) {
        success = ush_cmd_wait(arg);
    } else if (ush_streq(cmd, "sleep") != 0) {
        success = ush_cmd_sleep(arg);
    } else if (ush_streq(cmd, "yield") != 0) {
        success = ush_cmd_yield();
    } else if (ush_streq(cmd, "shutdown") != 0 || ush_streq(cmd, "poweroff") != 0) {
        success = ush_cmd_shutdown();
    } else if (ush_streq(cmd, "restart") != 0 || ush_streq(cmd, "reboot") != 0) {
        success = ush_cmd_restart();
    } else if (ush_streq(cmd, "exit") != 0) {
        success = ush_cmd_exit(sh, arg);
    } else if (ush_streq(cmd, "clear") != 0 || ush_streq(cmd, "cls") != 0) {
        success = ush_cmd_clear();
    } else if (ush_streq(cmd, "ansi") != 0 || ush_streq(cmd, "color") != 0) {
        success = ush_cmd_ansi();
    } else if (ush_streq(cmd, "ansitest") != 0) {
        success = ush_cmd_ansitest();
    } else if (ush_streq(cmd, "fastfetch") != 0) {
        success = ush_cmd_fastfetch(arg);
    } else if (ush_streq(cmd, "memstat") != 0) {
        success = ush_cmd_memstat();
    } else if (ush_streq(cmd, "fsstat") != 0) {
        success = ush_cmd_fsstat();
    } else if (ush_streq(cmd, "taskstat") != 0) {
        success = ush_cmd_taskstat();
    } else if (ush_streq(cmd, "userstat") != 0) {
        success = ush_cmd_userstat();
    } else if (ush_streq(cmd, "shstat") != 0) {
        success = ush_cmd_shstat(sh);
    } else if (ush_streq(cmd, "stats") != 0) {
        success = ush_cmd_stats(sh);
    } else if (ush_streq(cmd, "tty") != 0) {
        success = ush_cmd_tty(arg);
    } else if (ush_streq(cmd, "dmesg") != 0) {
        success = ush_cmd_dmesg(arg);
    } else if (ush_streq(cmd, "kbdstat") != 0) {
        success = ush_cmd_kbdstat();
    } else if (ush_streq(cmd, "mkdir") != 0) {
        success = ush_cmd_mkdir(sh, arg);
    } else if (ush_streq(cmd, "touch") != 0) {
        success = ush_cmd_touch(sh, arg);
    } else if (ush_streq(cmd, "write") != 0) {
        success = ush_cmd_write(sh, arg);
    } else if (ush_streq(cmd, "append") != 0) {
        success = ush_cmd_append(sh, arg);
    } else if (ush_streq(cmd, "cp") != 0) {
        success = ush_cmd_cp(sh, arg);
    } else if (ush_streq(cmd, "mv") != 0) {
        success = ush_cmd_mv(sh, arg);
    } else if (ush_streq(cmd, "rm") != 0) {
        success = ush_cmd_rm(sh, arg);
    } else if (ush_streq(cmd, "rusttest") != 0 || ush_streq(cmd, "panic") != 0 || ush_streq(cmd, "elfloader") != 0) {
        success = ush_cmd_not_supported(cmd, "this command is kernel-shell only");
    } else {
        known = 0;
        success = 0;
        ush_writeln("unknown command; type 'help'");
    }

    if (out_known != (int *)0) {
        *out_known = known;
    }

    if (out_success != (int *)0) {
        *out_success = success;
    }

    return 1;
}

static void ush_pipeline_set_stdin(const char *text, u64 len) {
    ush_pipeline_stdin_text = text;
    ush_pipeline_stdin_len = len;
}

static int ush_pipeline_has_meta(const char *line) {
    u64 i = 0ULL;

    if (line == (const char *)0) {
        return 0;
    }

    while (line[i] != '\0') {
        if (line[i] == '|' || line[i] == '>') {
            return 1;
        }
        i++;
    }

    return 0;
}

static int ush_pipeline_parse_stage(ush_pipeline_stage *stage, const char *segment_text) {
    char work[USH_LINE_MAX];
    char path_part[USH_PATH_MAX];
    const char *path_rest = "";
    u64 i;
    i64 op_pos = -1;
    int op_mode = 0;

    if (stage == (ush_pipeline_stage *)0 || segment_text == (const char *)0) {
        return 0;
    }

    ush_copy(work, (u64)sizeof(work), segment_text);
    ush_trim_line(work);

    if (work[0] == '\0') {
        return 0;
    }

    for (i = 0ULL; work[i] != '\0'; i++) {
        if (work[i] == '>') {
            if (op_pos >= 0) {
                ush_writeln("pipe: multiple redirections in one stage are not supported");
                return 0;
            }

            op_pos = (i64)i;

            if (work[i + 1ULL] == '>') {
                op_mode = 2;
                i++;
            } else {
                op_mode = 1;
            }
        }
    }

    stage->redirect_mode = 0;
    stage->redirect_path[0] = '\0';

    if (op_pos >= 0) {
        char *path_src;

        work[(u64)op_pos] = '\0';
        path_src = &work[(u64)op_pos + ((op_mode == 2) ? 2ULL : 1ULL)];

        ush_trim_line(work);
        ush_trim_line(path_src);

        if (path_src[0] == '\0') {
            ush_writeln("pipe: redirection path required");
            return 0;
        }

        if (ush_split_first_and_rest(path_src, path_part, (u64)sizeof(path_part), &path_rest) == 0) {
            ush_writeln("pipe: redirection path required");
            return 0;
        }

        if (path_rest != (const char *)0 && path_rest[0] != '\0') {
            ush_writeln("pipe: redirection path cannot contain spaces");
            return 0;
        }

        stage->redirect_mode = op_mode;
        ush_copy(stage->redirect_path, (u64)sizeof(stage->redirect_path), path_part);
    }

    ush_copy(stage->text, (u64)sizeof(stage->text), work);
    ush_parse_line(work, stage->cmd, (u64)sizeof(stage->cmd), stage->arg, (u64)sizeof(stage->arg));
    ush_trim_line(stage->arg);

    if (stage->cmd[0] == '\0') {
        ush_writeln("pipe: empty command stage");
        return 0;
    }

    return 1;
}

static int ush_pipeline_parse(const char *line,
                              ush_pipeline_stage *stages,
                              u64 max_stages,
                              u64 *out_stage_count) {
    char segment[USH_LINE_MAX];
    u64 i = 0ULL;
    u64 seg_pos = 0ULL;
    u64 stage_count = 0ULL;

    if (line == (const char *)0 || stages == (ush_pipeline_stage *)0 || max_stages == 0ULL || out_stage_count == (u64 *)0) {
        return 0;
    }

    *out_stage_count = 0ULL;

    for (;;) {
        char ch = line[i];

        if (ch == '|' || ch == '\0') {
            segment[seg_pos] = '\0';

            if (stage_count >= max_stages) {
                ush_writeln("pipe: too many stages");
                return 0;
            }

            if (ush_pipeline_parse_stage(&stages[stage_count], segment) == 0) {
                return 0;
            }

            stage_count++;
            seg_pos = 0ULL;

            if (ch == '\0') {
                break;
            }

            i++;
            continue;
        }

        if (seg_pos + 1ULL >= (u64)sizeof(segment)) {
            ush_writeln("pipe: stage text too long");
            return 0;
        }

        segment[seg_pos++] = ch;
        i++;
    }

    *out_stage_count = stage_count;
    return 1;
}

static int ush_pipeline_write_redirect(const ush_state *sh, const ush_pipeline_stage *stage, const char *data, u64 len) {
    char abs_path[USH_PATH_MAX];
    u64 ok;

    if (sh == (const ush_state *)0 || stage == (const ush_pipeline_stage *)0) {
        return 0;
    }

    if (stage->redirect_mode == 0) {
        return 1;
    }

    if (ush_resolve_path(sh, stage->redirect_path, abs_path, (u64)sizeof(abs_path)) == 0) {
        ush_writeln("redirect: invalid path");
        return 0;
    }

    if (stage->redirect_mode == 1) {
        ok = cleonos_sys_fs_write(abs_path, data, len);
    } else {
        ok = cleonos_sys_fs_append(abs_path, data, len);
    }

    if (ok == 0ULL) {
        ush_writeln("redirect: write failed");
        return 0;
    }

    return 1;
}

static int ush_execute_pipeline(ush_state *sh,
                                const char *line,
                                int *out_known,
                                int *out_success) {
    ush_pipeline_stage stages[USH_PIPELINE_MAX_STAGES];
    u64 stage_count = 0ULL;
    u64 i;
    const char *pipe_in = (const char *)0;
    u64 pipe_in_len = 0ULL;
    char *capture_out = ush_pipeline_capture_a;
    u64 capture_len = 0ULL;
    int known = 1;
    int success = 1;

    if (out_known != (int *)0) {
        *out_known = 1;
    }

    if (out_success != (int *)0) {
        *out_success = 0;
    }

    if (ush_pipeline_parse(line, stages, USH_PIPELINE_MAX_STAGES, &stage_count) == 0) {
        return 0;
    }

    for (i = 0ULL; i < stage_count; i++) {
        int stage_known = 1;
        int stage_success = 0;
        int mirror_to_tty = ((i + 1ULL) == stage_count && stages[i].redirect_mode == 0) ? 1 : 0;

        if (i + 1ULL < stage_count && stages[i].redirect_mode != 0) {
            ush_writeln("pipe: redirection is only supported on final stage");
            known = 1;
            success = 0;
            break;
        }

        ush_pipeline_set_stdin(pipe_in, pipe_in_len);
        ush_output_capture_begin(capture_out, (u64)USH_PIPE_CAPTURE_MAX + 1ULL, mirror_to_tty);
        (void)ush_execute_single_command(sh, stages[i].cmd, stages[i].arg, 0, &stage_known, &stage_success);
        capture_len = ush_output_capture_end();

        if (ush_output_capture_truncated() != 0) {
            ush_writeln("[pipe] captured output truncated");
        }

        if (stage_known == 0) {
            known = 0;
        }

        if (stage_success == 0) {
            success = 0;
            break;
        }

        if (stages[i].redirect_mode != 0) {
            if (ush_pipeline_write_redirect(sh, &stages[i], capture_out, capture_len) == 0) {
                success = 0;
                break;
            }
        }

        pipe_in = capture_out;
        pipe_in_len = capture_len;
        capture_out = (capture_out == ush_pipeline_capture_a) ? ush_pipeline_capture_b : ush_pipeline_capture_a;
    }

    ush_pipeline_set_stdin((const char *)0, 0ULL);

    if (out_known != (int *)0) {
        *out_known = known;
    }

    if (out_success != (int *)0) {
        *out_success = success;
    }

    return 1;
}

void ush_execute_line(ush_state *sh, const char *line) {
    char line_buf[USH_LINE_MAX];
    char cmd[USH_CMD_MAX];
    char arg[USH_ARG_MAX];
    u64 i = 0ULL;
    int known = 1;
    int success = 0;

    if (sh == (ush_state *)0 || line == (const char *)0) {
        return;
    }

    while (line[i] != '\0' && i + 1ULL < (u64)sizeof(line_buf)) {
        line_buf[i] = line[i];
        i++;
    }

    line_buf[i] = '\0';
    ush_trim_line(line_buf);

    if (line_buf[0] == '\0' || line_buf[0] == '#') {
        return;
    }

    if (ush_pipeline_has_meta(line_buf) != 0) {
        if (ush_execute_pipeline(sh, line_buf, &known, &success) == 0) {
            known = 1;
            success = 0;
        }
    } else {
        ush_parse_line(line_buf, cmd, (u64)sizeof(cmd), arg, (u64)sizeof(arg));
        ush_trim_line(arg);
        (void)ush_execute_single_command(sh, cmd, arg, 1, &known, &success);
    }

    sh->cmd_total++;

    if (success != 0) {
        sh->cmd_ok++;
    } else {
        sh->cmd_fail++;
    }

    if (known == 0) {
        sh->cmd_unknown++;
    }
}


