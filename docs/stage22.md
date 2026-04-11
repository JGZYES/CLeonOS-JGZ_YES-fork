# CLeonOS Stage22

## Stage Goal
- Add shell-side observability and diagnostics while keeping ELF path unchanged.
- Introduce in-kernel log journal so shell can inspect boot/runtime logs.
- Extend `/temp` workflow with copy/move utilities and command execution metrics.

## What Was Implemented
- Kernel log journal (ring buffer):
  - Added log journal storage in `clks/kernel/log.c`.
  - Added APIs:
    - `clks_log_journal_count()`
    - `clks_log_journal_read(index_from_oldest, out_line, out_size)`
- Shell command expansion:
  - `dmesg [n]` (reads recent journal lines)
  - `shstat` (shell command counters)
  - `memstat` (PMM + heap snapshot)
  - `fsstat` (VFS node and directory child counts)
  - `taskstat` (scheduler/task runtime snapshot)
  - `cp <src> <dst>`
  - `mv <src> <dst>`
- Command accounting:
  - Tracks total/ok/fail/unknown command counts.
- Safety policy:
  - `cp` destination must be under `/temp`.
  - `mv` source and destination must both be under `/temp`.
- Stage banner updated to:
  - `CLEONOS Stage22 START`

## Acceptance Criteria
- Boot completes and shell remains interactive.
- `help` lists new Stage22 commands.
- `dmesg` prints recent log lines from kernel journal.
- `shstat` shows changing counters after running commands.
- `memstat/fsstat/taskstat` print non-empty statistics.
- `/temp` copy/move flow works:
  - `cp` succeeds to `/temp` destination.
  - `mv` succeeds when both paths are in `/temp`.
  - invalid policy paths are rejected with message.

## Suggested Validation Script (Manual)
- `help`
- `dmesg 20`
- `memstat`
- `fsstat`
- `taskstat`
- `cd /temp`
- `mkdir s22`
- `write /temp/s22/a.txt alpha`
- `cp /temp/s22/a.txt /temp/s22/b.txt`
- `cat /temp/s22/b.txt`
- `mv /temp/s22/b.txt /temp/s22/c.txt`
- `ls /temp/s22`
- `shstat`

## Build Targets
- `make setup`
- `make userapps`
- `make iso`
- `make run`
- `make debug`

## QEMU Command
- `qemu-system-x86_64 -M q35 -m 1024M -cdrom build/CLeonOS-x86_64.iso -serial stdio`

## Common Bugs and Debugging
- `dmesg` output empty:
  - Ensure command is run after boot logs are produced.
- `cp` fails unexpectedly:
  - Check destination path is under `/temp`.
  - Verify source is a regular file.
- `mv` fails after copy:
  - Stage22 requires source path under `/temp` for removal.
- `taskstat` missing details:
  - Confirm scheduler task count is non-zero and system reached idle loop.