# CLeonOS 修改日志 - JGZ_YES Fork

## 提交信息

本次提交包含以下功能改进和新命令实现：

### 新增命令

1. **version 命令** (`cleonos/c/apps/version_main.c`)
   - 显示系统版本信息：`R-1-JGZ_YES`
   - 已在 help 命令中注册

2. **mkfile 命令** (`cleonos/c/apps/mkfile_main.c`)
   - 语法：`mkfile <文件名>`
   - 功能：在 `/temp` 目录下创建新文件
   - 使用 `cleonos_sys_fs_write` 系统调用创建空文件

3. **nano 命令** (`cleonos/c/apps/nano_main.c`)
   - 语法：`nano <文件名>`
   - 功能：文本编辑器
   - 支持快捷键：
     - `Ctrl+S`：保存文件
     - `Ctrl+X`：退出（需先按 Ctrl+D 结束输入）
     - `Ctrl+D`：结束输入并保存
     - `Ctrl+C`：强制退出，不保存
   - 编辑内容限制：最大 4096 字节

4. **sview 命令** (`cleonos/c/apps/sview_main.c`)
   - 语法：`sview <文件名>`
   - 功能：文本文件预览
   - 使用 `ush_writeln` 替代 `puts` 以兼容终端输出

### 功能修复

1. **修复 sview 命令无法显示文件内容**
   - 原因：使用 `puts()` 函数与系统终端不兼容
   - 解决方案：替换为 `ush_writeln()` 进行标准输出

2. **修复 nano 命令编译错误**
   - 问题：`cleonos_sys_fd_lseek` 函数未定义
   - 解决方案：通过关闭并重新打开文件（使用 `O_TRUNC` 标志）实现文件指针重置

### Shell 改进

1. **添加 Ctrl+C 强制退出功能** (`cleonos/c/apps/shell/shell_input.c`, `shell_internal.h`)
   - 在 `ush_state` 结构体中添加 `break_requested` 字段
   - 修改 `ush_read_char_blocking()` 函数支持中断检查
   - 在 `ush_read_line()` 中处理 Ctrl+C：清空当前行并重新显示提示符
   - 在 nano 编辑器中支持 Ctrl+C 强制退出

### 构建系统更新

1. **CMakeLists.txt**
   - 在 `USER_SHELL_COMMAND_APPS` 中添加新命令：`version`, `sview`, `mkfile`, `nano`

### 帮助信息更新

1. **help 命令** (`cleonos/c/apps/help_main.c`)
   - 添加对新命令的说明：
     - `version`：显示版本信息
     - `mkfile <file>`：创建新文件
     - `nano <file>`：文本编辑器
     - `sview <file>`：预览文件内容

## 技术细节

### 系统调用使用
- `cleonos_sys_fd_open`：打开/创建文件
- `cleonos_sys_fd_read`：读取文件内容或用户输入
- `cleonos_sys_fd_write`：写入文件
- `cleonos_sys_fd_close`：关闭文件描述符
- `cleonos_sys_fs_write`：创建空文件

### 文件路径限制
- `mkfile` 和 `nano` 命令要求目标文件必须在 `/temp` 目录下
- 使用 `ush_path_is_under_temp()` 函数进行路径验证

### 已知限制
- nano 编辑器使用独立的输入循环，不直接受 shell 的 `break_requested` 控制
- 文件大小限制为 4096 字节

## 作者

- 提交者：JGZ_YES
- 版本：R-1-JGZ_YES
