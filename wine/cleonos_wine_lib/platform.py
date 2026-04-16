from __future__ import annotations

import sys

try:
    from unicorn import Uc, UcError
    from unicorn import UC_ARCH_X86, UC_MODE_64
    from unicorn import UC_HOOK_CODE, UC_HOOK_INTR
    from unicorn import UC_PROT_ALL, UC_PROT_EXEC, UC_PROT_READ, UC_PROT_WRITE
    from unicorn import (
        UC_ERR_FETCH_PROT,
        UC_ERR_FETCH_UNMAPPED,
        UC_ERR_INSN_INVALID,
        UC_ERR_READ_PROT,
        UC_ERR_READ_UNMAPPED,
        UC_ERR_WRITE_PROT,
        UC_ERR_WRITE_UNMAPPED,
    )
    from unicorn.x86_const import (
        UC_X86_REG_RAX,
        UC_X86_REG_RBX,
        UC_X86_REG_RCX,
        UC_X86_REG_RDX,
        UC_X86_REG_RBP,
        UC_X86_REG_RIP,
        UC_X86_REG_RSP,
    )
except Exception as exc:
    print("[WINE][ERROR] unicorn import failed. Install dependencies first:", file=sys.stderr)
    print("  pip install -r wine/requirements.txt", file=sys.stderr)
    raise SystemExit(1) from exc


__all__ = [
    "Uc",
    "UcError",
    "UC_ARCH_X86",
    "UC_MODE_64",
    "UC_HOOK_CODE",
    "UC_HOOK_INTR",
    "UC_PROT_ALL",
    "UC_PROT_EXEC",
    "UC_PROT_READ",
    "UC_PROT_WRITE",
    "UC_ERR_FETCH_PROT",
    "UC_ERR_FETCH_UNMAPPED",
    "UC_ERR_INSN_INVALID",
    "UC_ERR_READ_PROT",
    "UC_ERR_READ_UNMAPPED",
    "UC_ERR_WRITE_PROT",
    "UC_ERR_WRITE_UNMAPPED",
    "UC_X86_REG_RAX",
    "UC_X86_REG_RBX",
    "UC_X86_REG_RCX",
    "UC_X86_REG_RDX",
    "UC_X86_REG_RBP",
    "UC_X86_REG_RIP",
    "UC_X86_REG_RSP",
]
