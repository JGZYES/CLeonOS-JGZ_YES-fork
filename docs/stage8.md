# CLeonOS Stage8

## Stage Goal
- Add kernel driver abstraction and registration framework.
- Register built-in drivers (serial, framebuffer, tty) under a unified manager.
- Probe `/driver` directory ELF drivers from ramdisk and validate ELF metadata.
- Extend user app packaging to include driver ELF in `/driver`.

## Acceptance Criteria
- Kernel boots and prints `CLEONOS STAGE8 START`.
- Driver framework logs `DRIVER MANAGER ONLINE`.
- Built-in drivers are registered and total driver count is logged.
- `/driver/*.elf` files are discovered, validated, and logged as `DRIVER ELF READY`.
- System continues to scheduler/interrupt/syscall idle loop without panic.

## Build Targets
- `make setup`
- `make userapps`
- `make iso`
- `make run`
- `make debug`

## QEMU Command
- `qemu-system-x86_64 -M q35 -m 1024M -cdrom build/CLeonOS-x86_64.iso -serial stdio`

## Common Bugs and Debugging
- `DRIVER ELF INVALID`:
  - Verify ELF is built as x86_64 ELF64 and has valid program headers.
- `DRIVER ELF MISSING`:
  - Ensure `make userapps` finished and ramdisk staging copied files to `/driver`.
- Driver count is lower than expected:
  - Check `clks_driver_init()` call order occurs after `clks_fs_init()`.
- Build failure for `ttydrv_main.c` symbols:
  - Confirm `cleonos/c/apps/ttydrv_main.c` exists and `USER_TTYDRV_OBJECT` is in Makefile.
- No driver logs on boot:
  - Confirm kernel includes `clks/kernel/driver.c` in `C_SOURCES`.
