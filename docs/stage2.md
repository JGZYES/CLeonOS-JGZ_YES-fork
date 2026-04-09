# CLeonOS Stage2

## Stage Goal
- Introduce foundational memory management for CLKS.
- Initialize physical memory manager (PMM) from Limine memmap.
- Add kernel heap allocator (`clks_kmalloc`/`clks_kfree`) without external libc.
- Emit memory statistics via kernel log during boot.

## Acceptance Criteria
- Kernel receives valid Limine memmap response.
- PMM initializes and prints managed/free/used/dropped page counts.
- Heap initializes and prints total/free bytes.
- `clks_kmalloc` self-test allocates and frees successfully.
- Kernel still reaches idle loop without panic.

## Build Targets
- `make setup`
- `make iso`
- `make run`
- `make debug`

## QEMU Command
- `qemu-system-x86_64 -M q35 -m 1024M -cdrom build/CLeonOS-x86_64.iso -serial stdio`

## Common Bugs and Debugging
- `NO LIMINE MEMMAP RESPONSE`:
  - Check Limine protocol entry in `configs/limine.conf` and ensure `protocol: limine` is used.
- PMM managed pages are 0:
  - Validate `LIMINE_MEMMAP_REQUEST` ID and section placement in `limine_requests.c`.
- Heap self-test failed:
  - Inspect `heap.c` block split/merge logic and verify allocator init was called.
- Limine panic about ELF segment permissions:
  - Ensure linker sections are page-aligned and segment permissions do not share pages.
- Limine artifacts missing during ISO packaging:
  - Verify `make setup` completed and `limine/bin/*` files exist.