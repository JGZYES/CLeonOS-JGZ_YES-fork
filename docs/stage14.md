# CLeonOS Stage14

## Stage Goal
- Pause risky ELF runtime expansion and prioritize non-ELF core capability.
- Expose virtual TTY manager controls through syscall ABI.
- Keep Stage13 boot flow stable while adding TTY observability and control hooks.

## What Was Added
- New kernel syscalls:
  - `CLKS_SYSCALL_TTY_COUNT` (21)
  - `CLKS_SYSCALL_TTY_ACTIVE` (22)
  - `CLKS_SYSCALL_TTY_SWITCH` (23)
- New TTY kernel API exports:
  - `clks_tty_count()`
  - `clks_tty_ready()`
- New user C wrappers:
  - `cleonos_sys_tty_count()`
  - `cleonos_sys_tty_active()`
  - `cleonos_sys_tty_switch(u64 tty_index)`
- Boot probe now logs TTY syscall status (`COUNT` and `ACTIVE`).

## Acceptance Criteria
- Kernel boots and prints `CLEONOS STAGE14 START`.
- Syscall framework remains online (`INT80 FRAMEWORK ONLINE`).
- Probe logs include:
  - `[INFO][TTY] COUNT: 0X...`
  - `[INFO][TTY] ACTIVE: 0X...`
- No regression in existing scheduler/service/FS/driver initialization sequence.

## Build Targets
- `make setup`
- `make userapps`
- `make iso`
- `make run`
- `make debug`

## QEMU Command
- `qemu-system-x86_64 -M q35 -m 1024M -cdrom build/CLeonOS-x86_64.iso -serial stdio`

## Common Bugs and Debugging
- TTY syscall value always `-1`:
  - Check kernel/user syscall ID tables are aligned.
- `TTY_ACTIVE` never changes after switch:
  - Validate `clks_tty_switch()` receives index `< clks_tty_count()`.
- Missing TTY probe logs:
  - Ensure `clks_stage14_syscall_probe()` is still called after `clks_syscall_init()` and `clks_interrupts_init()`.
- Build fails with undeclared syscall IDs:
  - Rebuild after syncing both headers:
    - `clks/include/clks/syscall.h`
    - `cleonos/c/include/cleonos_syscall.h`

