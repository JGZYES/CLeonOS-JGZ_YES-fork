# Diff Details

Date : 2026-04-17 16:44:24

Directory d:\\Projects\\C\\cleonos

Total : 80 files,  8302 codes, 1 comments, 2069 blanks, all 10372 lines

[Summary](results.md) / [Details](details.md) / [Diff Summary](diff.md) / Diff Details

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [.github/workflows/build-os.yml](/.github/workflows/build-os.yml) | YAML | 48 | 0 | 10 | 58 |
| [CMakeLists.txt](/CMakeLists.txt) | CMake | 8 | 0 | 2 | 10 |
| [README.md](/README.md) | Markdown | 92 | 0 | 34 | 126 |
| [README.zh-CN.md](/README.zh-CN.md) | Markdown | 92 | 0 | 34 | 126 |
| [cleonos/c/apps/ansi\_main.c](/cleonos/c/apps/ansi_main.c) | C | 39 | 0 | 8 | 47 |
| [cleonos/c/apps/ansitest\_main.c](/cleonos/c/apps/ansitest_main.c) | C | 142 | 0 | 35 | 177 |
| [cleonos/c/apps/append\_main.c](/cleonos/c/apps/append_main.c) | C | 70 | 0 | 15 | 85 |
| [cleonos/c/apps/args\_main.c](/cleonos/c/apps/args_main.c) | C | 49 | 0 | 13 | 62 |
| [cleonos/c/apps/cat\_main.c](/cleonos/c/apps/cat_main.c) | C | 79 | 0 | 21 | 100 |
| [cleonos/c/apps/cd\_main.c](/cleonos/c/apps/cd_main.c) | C | 50 | 0 | 12 | 62 |
| [cleonos/c/apps/clear\_main.c](/cleonos/c/apps/clear_main.c) | C | 35 | 0 | 8 | 43 |
| [cleonos/c/apps/cmd\_runtime.c](/cleonos/c/apps/cmd_runtime.c) | C | 446 | 0 | 143 | 589 |
| [cleonos/c/apps/cmd\_runtime.h](/cleonos/c/apps/cmd_runtime.h) | C++ | 80 | 0 | 19 | 99 |
| [cleonos/c/apps/cp\_main.c](/cleonos/c/apps/cp_main.c) | C | 91 | 0 | 23 | 114 |
| [cleonos/c/apps/cut\_main.c](/cleonos/c/apps/cut_main.c) | C | 214 | 1 | 55 | 270 |
| [cleonos/c/apps/dmesg\_main.c](/cleonos/c/apps/dmesg_main.c) | C | 60 | 0 | 15 | 75 |
| [cleonos/c/apps/exec\_main.c](/cleonos/c/apps/exec_main.c) | C | 83 | 0 | 19 | 102 |
| [cleonos/c/apps/exit\_main.c](/cleonos/c/apps/exit_main.c) | C | 50 | 0 | 11 | 61 |
| [cleonos/c/apps/fastfetch\_main.c](/cleonos/c/apps/fastfetch_main.c) | C | 180 | 0 | 34 | 214 |
| [cleonos/c/apps/fsstat\_main.c](/cleonos/c/apps/fsstat_main.c) | C | 41 | 0 | 8 | 49 |
| [cleonos/c/apps/grep\_main.c](/cleonos/c/apps/grep_main.c) | C | 195 | 0 | 54 | 249 |
| [cleonos/c/apps/head\_main.c](/cleonos/c/apps/head_main.c) | C | 164 | 0 | 47 | 211 |
| [cleonos/c/apps/help\_main.c](/cleonos/c/apps/help_main.c) | C | 71 | 0 | 7 | 78 |
| [cleonos/c/apps/kbdstat\_main.c](/cleonos/c/apps/kbdstat_main.c) | C | 40 | 0 | 8 | 48 |
| [cleonos/c/apps/ls\_main.c](/cleonos/c/apps/ls_main.c) | C | 258 | 0 | 73 | 331 |
| [cleonos/c/apps/memstat\_main.c](/cleonos/c/apps/memstat_main.c) | C | 39 | 0 | 8 | 47 |
| [cleonos/c/apps/mkdir\_main.c](/cleonos/c/apps/mkdir_main.c) | C | 53 | 0 | 13 | 66 |
| [cleonos/c/apps/mv\_main.c](/cleonos/c/apps/mv_main.c) | C | 98 | 0 | 25 | 123 |
| [cleonos/c/apps/pid\_main.c](/cleonos/c/apps/pid_main.c) | C | 35 | 0 | 8 | 43 |
| [cleonos/c/apps/pwd\_main.c](/cleonos/c/apps/pwd_main.c) | C | 35 | 0 | 8 | 43 |
| [cleonos/c/apps/restart\_main.c](/cleonos/c/apps/restart_main.c) | C | 36 | 0 | 8 | 44 |
| [cleonos/c/apps/rm\_main.c](/cleonos/c/apps/rm_main.c) | C | 53 | 0 | 13 | 66 |
| [cleonos/c/apps/shell/shell\_cmd.c](/cleonos/c/apps/shell/shell_cmd.c) | C | 1,535 | 0 | 392 | 1,927 |
| [cleonos/c/apps/shell/shell\_external.c](/cleonos/c/apps/shell/shell_external.c) | C | -57 | 0 | -20 | -77 |
| [cleonos/c/apps/shell/shell\_input.c](/cleonos/c/apps/shell/shell_input.c) | C | 259 | 0 | 64 | 323 |
| [cleonos/c/apps/shell/shell\_internal.h](/cleonos/c/apps/shell/shell_internal.h) | C++ | 8 | 0 | 1 | 9 |
| [cleonos/c/apps/shell/shell\_util.c](/cleonos/c/apps/shell/shell_util.c) | C | 72 | 0 | 20 | 92 |
| [cleonos/c/apps/shstat\_main.c](/cleonos/c/apps/shstat_main.c) | C | 41 | 0 | 8 | 49 |
| [cleonos/c/apps/shutdown\_main.c](/cleonos/c/apps/shutdown_main.c) | C | 36 | 0 | 8 | 44 |
| [cleonos/c/apps/sleep\_main.c](/cleonos/c/apps/sleep_main.c) | C | 48 | 0 | 11 | 59 |
| [cleonos/c/apps/sort\_main.c](/cleonos/c/apps/sort_main.c) | C | 205 | 0 | 63 | 268 |
| [cleonos/c/apps/spawn\_main.c](/cleonos/c/apps/spawn_main.c) | C | 72 | 0 | 17 | 89 |
| [cleonos/c/apps/stats\_main.c](/cleonos/c/apps/stats_main.c) | C | 98 | 0 | 14 | 112 |
| [cleonos/c/apps/tail\_main.c](/cleonos/c/apps/tail_main.c) | C | 190 | 0 | 54 | 244 |
| [cleonos/c/apps/taskstat\_main.c](/cleonos/c/apps/taskstat_main.c) | C | 39 | 0 | 8 | 47 |
| [cleonos/c/apps/touch\_main.c](/cleonos/c/apps/touch_main.c) | C | 54 | 0 | 13 | 67 |
| [cleonos/c/apps/tty\_main.c](/cleonos/c/apps/tty_main.c) | C | 60 | 0 | 14 | 74 |
| [cleonos/c/apps/uniq\_main.c](/cleonos/c/apps/uniq_main.c) | C | 168 | 0 | 48 | 216 |
| [cleonos/c/apps/userstat\_main.c](/cleonos/c/apps/userstat_main.c) | C | 44 | 0 | 8 | 52 |
| [cleonos/c/apps/wait\_main.c](/cleonos/c/apps/wait_main.c) | C | 64 | 0 | 14 | 78 |
| [cleonos/c/apps/wavplay\_main.c](/cleonos/c/apps/wavplay_main.c) | C | 396 | 0 | 99 | 495 |
| [cleonos/c/apps/wc\_main.c](/cleonos/c/apps/wc_main.c) | C | 164 | 0 | 45 | 209 |
| [cleonos/c/apps/write\_main.c](/cleonos/c/apps/write_main.c) | C | 70 | 0 | 15 | 85 |
| [cleonos/c/apps/yield\_main.c](/cleonos/c/apps/yield_main.c) | C | 35 | 0 | 8 | 43 |
| [cleonos/c/include/cleonos\_syscall.h](/cleonos/c/include/cleonos_syscall.h) | C++ | 26 | 0 | 0 | 26 |
| [cleonos/c/src/runtime.c](/cleonos/c/src/runtime.c) | C | 29 | 0 | 7 | 36 |
| [cleonos/c/src/syscall.c](/cleonos/c/src/syscall.c) | C | 39 | 0 | 14 | 53 |
| [clks/arch/x86\_64/exec\_stack\_call.S](/clks/arch/x86_64/exec_stack_call.S) | Go Assembly | 8 | 0 | 2 | 10 |
| [clks/drivers/audio/pcspeaker.c](/clks/drivers/audio/pcspeaker.c) | C | 101 | 0 | 25 | 126 |
| [clks/drivers/video/framebuffer.c](/clks/drivers/video/framebuffer.c) | C | 21 | 0 | 6 | 27 |
| [clks/include/clks/audio.h](/clks/include/clks/audio.h) | C++ | 9 | 0 | 3 | 12 |
| [clks/include/clks/driver.h](/clks/include/clks/driver.h) | C++ | 1 | 0 | 0 | 1 |
| [clks/include/clks/exec.h](/clks/include/clks/exec.h) | C++ | 16 | 0 | 0 | 16 |
| [clks/include/clks/framebuffer.h](/clks/include/clks/framebuffer.h) | C++ | 3 | 0 | 1 | 4 |
| [clks/include/clks/keyboard.h](/clks/include/clks/keyboard.h) | C++ | 7 | 0 | 1 | 8 |
| [clks/include/clks/syscall.h](/clks/include/clks/syscall.h) | C++ | 13 | 0 | 1 | 14 |
| [clks/kernel/driver.c](/clks/kernel/driver.c) | C | 6 | 0 | 1 | 7 |
| [clks/kernel/exec.c](/clks/kernel/exec.c) | C | 299 | 0 | 71 | 370 |
| [clks/kernel/interrupts.c](/clks/kernel/interrupts.c) | C | 9 | 0 | 1 | 10 |
| [clks/kernel/keyboard.c](/clks/kernel/keyboard.c) | C | 52 | 0 | 15 | 67 |
| [clks/kernel/kmain.c](/clks/kernel/kmain.c) | C | 2 | 0 | 0 | 2 |
| [clks/kernel/syscall.c](/clks/kernel/syscall.c) | C | 127 | 0 | 30 | 157 |
| [clks/kernel/tty.c](/clks/kernel/tty.c) | C | 486 | 0 | 108 | 594 |
| [docs/stage27.md](/docs/stage27.md) | Markdown | 4 | 0 | 0 | 4 |
| [docs/syscall.md](/docs/syscall.md) | Markdown | 77 | 0 | 30 | 107 |
| [wine/README.md](/wine/README.md) | Markdown | 12 | 0 | 5 | 17 |
| [wine/cleonos\_wine\_lib/constants.py](/wine/cleonos_wine_lib/constants.py) | Python | 13 | 0 | 1 | 14 |
| [wine/cleonos\_wine\_lib/platform.py](/wine/cleonos_wine_lib/platform.py) | Python | 18 | 0 | 1 | 19 |
| [wine/cleonos\_wine\_lib/runner.py](/wine/cleonos_wine_lib/runner.py) | Python | 237 | 0 | 38 | 275 |
| [wine/cleonos\_wine\_lib/state.py](/wine/cleonos_wine_lib/state.py) | Python | 60 | 0 | 13 | 73 |

[Summary](results.md) / [Details](details.md) / [Diff Summary](diff.md) / Diff Details