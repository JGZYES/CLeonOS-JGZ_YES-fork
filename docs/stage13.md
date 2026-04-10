# CLeonOS Stage13

## Stage Goal
- Upgrade shell command framework from hardcoded commands to script-driven execution.
- Add `/shell/init.cmd` startup script support with comment/blank-line handling.
- Extend user C syscall wrappers for runtime observability commands (`stats`).
- Keep Stage12 user execution manager stable while improving shell orchestration.

## Acceptance Criteria
- Kernel boots and prints `CLEONOS STAGE13 START`.
- Userland logs include init script detection:
  - `INIT SCRIPT READY /SHELL/INIT.CMD` and script size.
- Shell logs show script mode:
  - `[USER][SHELL] script /shell/init.cmd`
  - command lines executed from script (`$ help`, `$ stats`, etc.).
- `stats` command prints runtime counters via syscall wrappers.
- System remains stable in idle loop.

## Build Targets
- `make setup`
- `make userapps`
- `make iso`
- `make run`
- `make debug`

## QEMU Command
- `qemu-system-x86_64 -M q35 -m 1024M -cdrom build/CLeonOS-x86_64.iso -serial stdio`

## Common Bugs and Debugging
- Shell falls back to default script:
  - Check `ramdisk/shell/init.cmd` exists and is packed into ramdisk.
- `stats` values not updated:
  - Confirm syscall IDs and wrappers remain aligned between kernel/user headers.
- Script command ignored unexpectedly:
  - Verify command is not prefixed with `#` and does not exceed line buffer.
- `cat` output truncated:
  - Current stage intentionally limits `cat` output to `SHELL_CAT_MAX` bytes.
- Boot regression after Stage13 merge:
  - Re-check sequence: exec init -> userland init -> scheduler/services -> interrupts.
