#include "cmd_runtime.h"

#define NANO_BUFFER_SIZE 4096

static int ush_cmd_nano(const ush_state *sh, const char *arg) {
    char path[USH_PATH_MAX];
    char buffer[NANO_BUFFER_SIZE];
    u64 fd;
    u64 size;

    if (arg == (const char *)0 || arg[0] == '\0') {
        ush_writeln("nano: file path required");
        return 0;
    }

    if (ush_resolve_path(sh, arg, path, (u64)sizeof(path)) == 0) {
        ush_writeln("nano: invalid path");
        return 0;
    }

    if (ush_path_is_under_temp(path) == 0) {
        ush_writeln("nano: target must be under /temp");
        return 0;
    }

    fd = cleonos_sys_fd_open(path, CLEONOS_O_RDWR | CLEONOS_O_CREAT, 0ULL);
    if (fd == (u64)-1) {
        ush_writeln("nano: open failed");
        return 0;
    }

    ush_writeln("nano text editor");
    ush_writeln("Press Ctrl+X to exit, Ctrl+S to save");
    ush_writeln("");

    size = cleonos_sys_fd_read(fd, buffer, (u64)sizeof(buffer) - 1);
    if (size == (u64)-1) {
        (void)cleonos_sys_fd_close(fd);
        ush_writeln("nano: read failed");
        return 0;
    }
    buffer[size] = '\0';

    ush_writeln("Current content:");
    ush_writeln(buffer);
    ush_writeln("");
    ush_writeln("Enter new content (end with Ctrl+D):");

    size = 0;
    while (size < (u64)sizeof(buffer) - 1) {
        char c;
        u64 read = cleonos_sys_fd_read(0ULL, &c, 1ULL);
        if (read == 0) {
            continue;
        }
        if (c == 3) {
            (void)cleonos_sys_fd_close(fd);
            ush_writeln("\n^C");
            ush_writeln("Exited without saving");
            return 1;
        }
        if (c == 4 && size > 0) {
            break;
        }
        if (c == 24) {
            (void)cleonos_sys_fd_close(fd);
            ush_writeln("\nExited without saving");
            return 1;
        }
        if (c == 19) {
            (void)cleonos_sys_fd_close(fd);
            fd = cleonos_sys_fd_open(path, CLEONOS_O_RDWR | CLEONOS_O_TRUNC, 0ULL);
            if (fd != (u64)-1) {
                (void)cleonos_sys_fd_write(fd, buffer, size);
                ush_writeln("\nSaved");
            } else {
                ush_writeln("\nSave failed");
            }
            continue;
        }
        if (c == '\r') {
            continue;
        }
        if (c == '\b' && size > 0) {
            size--;
            continue;
        }
        buffer[size++] = c;
    }
    buffer[size] = '\0';

    (void)cleonos_sys_fd_close(fd);
    fd = cleonos_sys_fd_open(path, CLEONOS_O_RDWR | CLEONOS_O_TRUNC, 0ULL);
    if (fd != (u64)-1) {
        (void)cleonos_sys_fd_write(fd, buffer, size);
        (void)cleonos_sys_fd_close(fd);
    }

    ush_writeln("\nFile saved");
    return 1;
}

int cleonos_app_main(void) {
    ush_cmd_ctx ctx;
    ush_cmd_ret ret;
    ush_state sh;
    char initial_cwd[USH_PATH_MAX];
    int has_context = 0;
    int success = 0;
    const char *arg = "";

    ush_zero(&ctx, (u64)sizeof(ctx));
    ush_zero(&ret, (u64)sizeof(ret));
    ush_init_state(&sh);
    ush_copy(initial_cwd, (u64)sizeof(initial_cwd), sh.cwd);

    if (ush_command_ctx_read(&ctx) != 0) {
        if (ctx.cmd[0] != '\0' && ush_streq(ctx.cmd, "nano") != 0) {
            has_context = 1;
            arg = ctx.arg;
            if (ctx.cwd[0] == '/') {
                ush_copy(sh.cwd, (u64)sizeof(sh.cwd), ctx.cwd);
                ush_copy(initial_cwd, (u64)sizeof(initial_cwd), sh.cwd);
            }
        }
    }

    success = ush_cmd_nano(&sh, arg);

    if (has_context != 0) {
        if (ush_streq(sh.cwd, initial_cwd) == 0) {
            ret.flags |= USH_CMD_RET_FLAG_CWD;
            ush_copy(ret.cwd, (u64)sizeof(ret.cwd), sh.cwd);
        }

        if (sh.exit_requested != 0) {
            ret.flags |= USH_CMD_RET_FLAG_EXIT;
            ret.exit_code = sh.exit_code;
        }

        (void)ush_command_ret_write(&ret);
    }

    return (success != 0) ? 0 : 1;
}