static int ush_cmd_pwd(const ush_state *sh) {
    ush_writeln(sh->cwd);
    return 1;
}
