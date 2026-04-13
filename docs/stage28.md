# Stage 28 - ANSI TTY + Default User Shell Entry + User Syscall Trace

## Goal
- Add ANSI color control support to TTY.
- Introduce ANSI-colored experience in user shell.
- Default to entering user shell after kernel is ready.
- Add serial log tracing for syscalls while user programs are running.

## Implementation
- TTY:
  - Added ANSI CSI/SGR parsing (`ESC[...m`) for standard foreground/background colors.
  - Added support for reset/default color behavior (`0`, `39`, `49`) and bright color range (`90..97`, `100..107`).
  - Added per-cell color buffers so scrolling/redraw keeps color attributes.
- User shell:
  - Colored prompt and startup text now use ANSI sequences.
  - Added `ansi`/`color` command to preview palette.
  - `clear` now uses ANSI clear/home sequence.
- Boot behavior:
  - Enabled user shell auto-exec by default in userland manager.
  - Boot log now reports default user-shell mode when auto-exec is enabled.
- Syscall tracing:
  - Added bounded debug trace in `clks/kernel/syscall.c` for syscalls issued while a user-path program is active.
  - Trace emits begin/end markers and syscall IDs with budget guard to avoid log flooding.

## Acceptance Criteria
- User shell prompt/output can display ANSI colors correctly on TTY.
- Boot enters default user-shell flow (auto launch enabled).
- Serial logs include syscall trace lines while user programs execute.
- Stage27 and Stage28 docs are present and indexed in `docs/README.md`.

## Build Targets
- `make userapps`
- `make ramdisk`
- `make iso`
- `make run`

## QEMU Command
- `make run`

## Debug Notes
- If ANSI text shows raw escape chars, verify `clks/kernel/tty.c` ANSI parser path is compiled.
- If user shell does not auto launch, check `clks/kernel/userland.c` auto-exec flags and retry tick flow.
- If syscall trace is missing, confirm `clks_exec_current_path_is_user()` is true during the target run.
