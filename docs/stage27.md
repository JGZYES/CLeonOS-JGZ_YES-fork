# Stage 27 - User Shell/Wine Split and Syscall Expansion

## Goal
- Expand user shell capability toward kernel-shell parity.
- Extend syscall ABI for richer process/runtime operations.
- Split CLeonOS-Wine implementation into maintainable modules.

## Implementation
- Added more user-facing commands in user shell around process/runtime control (`spawn`, `wait`, `sleep`, `yield`, `pid`, file ops under `/temp`).
- Extended syscall surface (process and runtime related IDs) and kept kernel/user syscall ID tables aligned.
- Added process argument/environment ABI (`EXEC_PATHV` / `SPAWN_PATHV` + `PROC_ARG*` / `PROC_ENV*`).
- Added user exception/fault reporting ABI (`PROC_LAST_SIGNAL` / `PROC_FAULT_*`) and signal-encoded exit status.
- Updated user runtime syscall wrappers to cover newly added syscall IDs.
- Refactored Wine codebase from single-file implementation into modular structure for CLI, ELF loader, syscall bridge, and runtime helpers.

## Acceptance Criteria
- User shell can complete process and temp-file workflows without falling back to kernel shell.
- User ELF can read `argc/argv/envp` through runtime startup path.
- `wait` can observe signal-encoded status for crashed user processes.
- New syscall IDs are available consistently in:
  - `clks/include/clks/syscall.h`
  - `cleonos/c/include/cleonos_syscall.h`
  - `cleonos/c/src/syscall.c`
- Wine entry remains runnable after split (`python cleonos_wine.py ...`) and supports process/argv/env/fault syscalls.

## Build Targets
- `make userapps`
- `make ramdisk`
- `make iso`
- `make run`

## QEMU Command
- `make run`

## Debug Notes
- If a user command reports `request failed`, verify syscall ID mapping first.
- If `wait`/`spawn` behavior is wrong, check `clks/kernel/exec.c` process table and pid stack.
- If Wine import fails after split, check package/module import paths and `__init__.py` exports.
