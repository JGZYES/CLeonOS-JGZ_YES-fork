# CLeonOS Stage11

## Stage Goal
- Build Stage11 shell command framework around syscall ABI (`help/ls/cat/run`).
- Extend INT80 syscall table with filesystem query/read operations.
- Add kernel `exec` path framework for ELF-by-path request validation.
- Add kernel-side syscall probe flow to verify command-path ABI during boot.

## Acceptance Criteria
- Kernel boots and prints `CLEONOS STAGE11 START`.
- Userland init logs `SHELL COMMAND ABI READY`.
- Exec framework logs `PATH EXEC FRAMEWORK ONLINE`.
- After interrupt/syscall online, Stage11 probe logs include:
  - `SHELL ROOT_CHILDREN`
  - `SHELL ROOT_ENTRY0`
  - `SHELL README PREVIEW`
  - `EXEC RUN /SYSTEM/ELFRUNNER.ELF OK`
  - `EXEC REQUESTS` and `EXEC SUCCESS`
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
- `undefined reference` for exec APIs:
  - Ensure `clks/kernel/exec.c` is listed in `C_SOURCES`.
- Stage11 syscall logs missing:
  - Verify `clks_stage11_syscall_probe()` is called after `clks_interrupts_init()`.
- `EXEC ELF INVALID` on run path:
  - Check `/system/*.elf` are valid ELF64 images in ramdisk.
- `ls`/`cat` shell syscalls return empty:
  - Confirm syscall IDs in `clks/include/clks/syscall.h` and `cleonos/c/include/cleonos_syscall.h` are identical.
- Shell app build fails with warnings:
  - Keep helper functions used and avoid implicit type conversions under `-Werror`.