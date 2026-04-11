# CLeonOS Stage20

## Stage Goal
- Keep system in kernel-shell mode while ELF user shell remains deferred.
- Stabilize terminal interaction path: PS/2 input queue + line editing shell loop.
- Enable external PSF font loading for framebuffer TTY.
- Keep scheduler/service/syscall/interrupt pipeline stable.

## What Was Implemented
- Boot pipeline keeps kernel shell as the active interactive interface.
- TTY supports loading external PSF font from `/system/tty.psf`.
- Keyboard module provides:
  - PS/2 scan code handling
  - input queue buffering
  - `Alt+F1..F4` virtual TTY switch hotkeys
- Kernel shell interactive loop is online:
  - command dispatch (`help/ls/cat/exec/clear/kbdstat` baseline)
  - line editing and history
- TTY cursor path is timer-driven and supports blink updates via `clks_tty_tick()`.

## Acceptance Criteria
- Kernel boots and prints `CLEONOS Stage20 START`.
- Boot logs include:
  - `[INFO][TTY] EXTERNAL PSF LOADED /SYSTEM/TTY.PSF` (or fallback warning)
  - `[INFO][KBD] ALT+F1..F4 TTY HOTKEY ONLINE`
  - `[INFO][KBD] PS2 INPUT QUEUE ONLINE`
  - `[INFO][SHELL] INTERACTIVE LOOP ONLINE`
- Typing in kernel shell produces immediate echo and command execution.
- `Alt+F1..F4` switches active TTY without panic.

## Build Targets
- `make setup`
- `make userapps`
- `make iso`
- `make run`
- `make debug`

## QEMU Command
- `qemu-system-x86_64 -M q35 -m 1024M -cdrom build/CLeonOS-x86_64.iso -serial stdio`

## Common Bugs and Debugging
- Cannot type in shell:
  - Ensure QEMU window has keyboard focus.
  - Check logs for keyboard queue online messages.
- External PSF not applied:
  - Verify `/system/tty.psf` exists in ramdisk package.
  - If load fails, system will fall back to built-in font.
- Hotkey switching does not work:
  - Confirm `Alt` is held while pressing `F1..F4`.
  - Check `[INFO][TTY] HOTKEY SWITCH` logs.
- Input lag feels high:
  - Increase shell input budget or scheduler frequency in later stage.