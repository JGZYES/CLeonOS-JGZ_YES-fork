#include <cleonos_syscall.h>

static const char ttydrv_banner[] = "[DRIVER][TTYDRV] ttydrv.elf online";

int cleonos_app_main(void) {
    cleonos_sys_log_write(ttydrv_banner, (u64)(sizeof(ttydrv_banner) - 1U));
    return 0;
}
