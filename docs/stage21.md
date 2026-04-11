# CLeonOS Stage21

## Stage Goal
- Add writable runtime filesystem capability without touching ELF execution path.
- Restrict write operations to `/temp` to keep system area read-only.
- Extend kernel shell with practical file/dir operations for daily debugging.

## What Was Implemented
- New writable VFS APIs:
  - `clks_fs_mkdir(const char *path)`
  - `clks_fs_write_all(const char *path, const void *data, u64 size)`
  - `clks_fs_append(const char *path, const void *data, u64 size)`
  - `clks_fs_remove(const char *path)`
- Write guard policy:
  - Only `/temp` subtree is writable.
  - `/system`, `/shell`, `/driver` remain read-only.
  - Non-empty directory delete is rejected.
- Runtime file payload management:
  - Dynamically written file data uses kernel heap.
  - Overwrite/append paths release previous heap-backed payload safely.
- Kernel shell command expansion:
  - `pwd`
  - `cd [dir]`
  - `mkdir <dir>`
  - `touch <file>`
  - `write <file> <text>`
  - `append <file> <text>`
  - `rm <path>`
- Shell path handling upgraded:
  - relative path support
  - `.` and `..` handling
  - shell-maintained current working directory

## Acceptance Criteria
- Boot completes and kernel shell is interactive.
- `help` output includes new file commands.
- Writable flow under `/temp` works:
  - create dir
  - create file
  - write and append text
  - read with `cat`
  - remove file/empty dir
- Write operation outside `/temp` fails with safe message.
- No regression in scheduler/service/syscall/interrupt initialization logs.

## Suggested Validation Script (Manual)
- `pwd`
- `cd /temp`
- `mkdir demo`
- `cd demo`
- `touch a.txt`
- `write a.txt hello`
- `append a.txt _world`
- `cat a.txt`  (expect `hello_world`)
- `cd /`
- `write /system/x.txt bad`  (expect failure)
- `rm /temp/demo/a.txt`
- `rm /temp/demo`

## Build Targets
- `make setup`
- `make userapps`
- `make iso`
- `make run`
- `make debug`

## QEMU Command
- `qemu-system-x86_64 -M q35 -m 1024M -cdrom build/CLeonOS-x86_64.iso -serial stdio`

## Common Bugs and Debugging
- `mkdir/write/touch` always fails:
  - Confirm target path resolves under `/temp`.
  - Use `pwd` and absolute paths to verify current directory.
- `rm` fails for directory:
  - Stage21 only allows removing empty directories.
- `cat` prints truncated data:
  - Current shell output keeps bounded preview buffer by design.
- Unexpected write behavior after repeated overwrite:
  - Check heap stats and ensure no panic; Stage21 releases old heap payload before replacing.