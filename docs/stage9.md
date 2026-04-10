# CLeonOS Stage9

## Stage Goal
- Add kernel service layer to represent core kernel capabilities.
- Extend scheduler with real task execution callbacks on timer ticks.
- Connect worker tasks (`klogd`, `kworker`) to service heartbeat updates.
- Extend syscall interface with service and scheduler runtime counters.

## Acceptance Criteria
- Kernel boots and prints `CLEONOS STAGE9 START`.
- Service framework logs `KERNEL SERVICES ONLINE`.
- Service counters (`COUNT`, `READY`) are logged.
- Scheduler still reports task count and runs without panic.
- Syscall layer remains online and returns valid ticks after interrupt init.

## Build Targets
- `make setup`
- `make userapps`
- `make iso`
- `make run`
- `make debug`

## QEMU Command
- `qemu-system-x86_64 -M q35 -m 1024M -cdrom build/CLeonOS-x86_64.iso -serial stdio`

## Common Bugs and Debugging
- `undefined reference` for service APIs:
  - Ensure `clks/kernel/service.c` is listed in `C_SOURCES`.
- Task callbacks never run:
  - Confirm timer IRQ path still calls `clks_scheduler_on_timer_tick()`.
- Service count is 0:
  - Verify `clks_service_init()` is called after scheduler/driver/fs init.
- Syscall service counters return `-1`:
  - Check syscall IDs in kernel and user headers match.
- Boot panic after Stage9 merge:
  - Re-check `kmain` init order: FS -> userland -> driver -> scheduler -> service -> interrupts.
