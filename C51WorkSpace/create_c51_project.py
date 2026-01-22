#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
from pathlib import Path

# 颜色定义
class Colors:
    GREEN = '\033[0;32m'
    BLUE = '\033[0;34m'
    YELLOW = '\033[1;33m'
    RED = '\033[0;31m'
    NC = '\033[0m'  # No Color

def print_colored(text, color=Colors.NC):
    print(f"{color}{text}{Colors.NC}")

def create_project_structure(project_name):
    """创建项目文件夹结构"""
    project_path = Path(project_name)
    
    # 检查项目是否已存在
    if project_path.exists():
        print_colored(f"警告: 项目文件夹 '{project_name}' 已存在", Colors.YELLOW)
        response = input("是否继续? (y/n): ")
        if response.lower() != 'y':
            print_colored("操作已取消", Colors.RED)
            sys.exit(1)
    
    # 创建文件夹结构
    dirs = [
        project_path / "src",
        project_path / "include",
        project_path / "lib",
        project_path / "build",
        project_path / ".vscode"
    ]
    
    for dir_path in dirs:
        dir_path.mkdir(parents=True, exist_ok=True)
        print_colored(f"✓ 创建文件夹: {dir_path}", Colors.GREEN)
    
    return project_path

def create_makefile(project_path, project_name):
    """创建 Makefile"""
    makefile_content = f'''# Makefile for {project_name}
# SDCC C51 项目构建配置

# 项目名称
PROJECT = {project_name}

# 源文件目录
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
BUILD_DIR = build

# 源文件
SOURCES = $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.asm)

# 将 .c 文件转换为 .rel 文件（目标文件）
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.rel)

# 头文件路径
INCLUDES = -I$(INC_DIR) -I$(SRC_DIR)

# SDCC 编译器选项
# -mmcs51: 使用 MCS-51 架构
# --model-small: 小内存模型 (128字节内部RAM)
# --std-c99: 使用 C99 标准
# --opt-code-size: 优化代码大小
# --nooverlay: 不使用覆盖段
SDCC_FLAGS = -mmcs51 --model-small --std-c99 --opt-code-size --nooverlay
SDCC_FLAGS += $(INCLUDES)

# 链接器选项
LINKER_FLAGS = -mmcs51 --model-small --code-loc 0x0000 --code-size 0x2000

# 输出文件
HEX_FILE = $(BUILD_DIR)/$(PROJECT).ihx
HEX_FILE_ABS = $(abspath $(HEX_FILE))

# 默认目标
all: $(HEX_FILE)
\t@echo "构建完成: $(HEX_FILE)"

# 链接所有目标文件生成 .ihx 文件
$(HEX_FILE): $(OBJECTS)
\t@mkdir -p $(BUILD_DIR)
\t@echo "正在链接..."
\tsdcc $(LINKER_FLAGS) -o $(HEX_FILE) $(OBJECTS)
\t@echo "链接完成: $(HEX_FILE)"

# 编译单个 C 文件为 .rel 文件
$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c
\t@mkdir -p $(BUILD_DIR)
\t@echo "正在编译: $<"
\tsdcc $(SDCC_FLAGS) -c -o $(BUILD_DIR)/ $<

# 清理构建文件
clean:
\t@echo "正在清理..."
\trm -rf $(BUILD_DIR)/*.ihx $(BUILD_DIR)/*.lk $(BUILD_DIR)/*.lst
\trm -rf $(BUILD_DIR)/*.map $(BUILD_DIR)/*.mem $(BUILD_DIR)/*.rel
\trm -rf $(BUILD_DIR)/*.rst $(BUILD_DIR)/*.sym $(BUILD_DIR)/*.asm
\trm -rf $(BUILD_DIR)/*.adb $(BUILD_DIR)/*.cdb
\t@echo "清理完成"

# 烧录到单片机 (需要根据实际情况修改串口)
# 使用方法: make flash PORT=/dev/ttyUSB0
flash: $(HEX_FILE)
\t@if [ -z "$(PORT)" ]; then \\
\t\techo "错误: 请指定串口设备，例如: make flash PORT=/dev/ttyUSB0"; \\
\t\texit 1; \\
\tfi
\t@echo "正在烧录到 $(PORT)..."
\tstcgal -p $(PORT) -P stc89 $(HEX_FILE_ABS)
\t@echo "烧录完成"

# 显示帮助信息
help:
\t@echo "可用目标:"
\t@echo "  make          - 编译项目"
\t@echo "  make clean    - 清理构建文件"
\t@echo "  make flash    - 烧录到单片机 (需要 PORT=串口设备)"
\t@echo "  make help     - 显示此帮助信息"
\t@echo ""
\t@echo "示例:"
\t@echo "  make"
\t@echo "  make flash PORT=/dev/ttyUSB0"
\t@echo "  make clean"

.PHONY: all clean flash help
'''
    makefile_file = project_path / "Makefile"
    makefile_file.write_text(makefile_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {makefile_file}", Colors.GREEN)

def create_build_sh(project_path, project_name):
    """创建 build.sh 脚本"""
    build_sh_content = f'''#!/bin/bash
# {project_name} 构建和烧录脚本

# 默认串口设备
PORT="/dev/ttyUSB0"

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--port)
            PORT="$2"
            shift 2
            ;;
        -c|--clean)
            echo "正在清理..."
            rm -rf build/*.ihx build/*.lk build/*.lst build/*.map
            rm -rf build/*.mem build/*.rel build/*.rst build/*.sym build/*.asm
            echo "清理完成"
            exit 0
            ;;
        -h|--help)
            echo "用法: $0 [选项]"
            echo "选项:"
            echo "  -p, --port PORT    指定串口设备 (默认: /dev/ttyUSB0)"
            echo "  -c, --clean        清理构建文件"
            echo "  -h, --help         显示帮助信息"
            exit 0
            ;;
        *)
            echo "未知选项: $1"
            echo "使用 -h 或 --help 查看帮助"
            exit 1
            ;;
    esac
done

# 创建构建目录
mkdir -p build

# 编译所有源文件为 .rel 文件
echo "正在编译源文件..."
for src_file in src/*.c; do
    if [ -f "$src_file" ]; then
        filename=$(basename "$src_file" .c)
        echo "  编译: $src_file"
        sdcc -mmcs51 --model-small --std-c99 --opt-code-size --nooverlay \\
             -Iinclude -Isrc \\
             -c -o build/ "$src_file"
        if [ $? -ne 0 ]; then
            echo "编译失败: $src_file"
            exit 1
        fi
    fi
done

# 链接所有 .rel 文件
echo "正在链接..."
sdcc -mmcs51 --model-small --code-loc 0x0000 --code-size 0x2000 \\
     -o "build/{project_name}.ihx" build/*.rel

if [ $? -ne 0 ]; then
    echo "链接失败!"
    exit 1
fi

echo "编译完成: build/{project_name}.ihx"

# 询问是否烧录
read -p "是否烧录到单片机? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "正在烧录到 $PORT..."
    stcgal -p "$PORT" -P stc89 "build/{project_name}.ihx"
    if [ $? -eq 0 ]; then
        echo "烧录完成!"
    else
        echo "烧录失败!"
        exit 1
    fi
fi
'''
    build_sh_file = project_path / "build.sh"
    build_sh_file.write_text(build_sh_content, encoding='utf-8')
    # 添加执行权限
    os.chmod(build_sh_file, 0o755)
    print_colored(f"✓ 创建文件: {build_sh_file}", Colors.GREEN)

def create_tasks_json(project_path):
    """创建 tasks.json"""
    tasks_content = '''{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "C51: Build",
      "type": "shell",
      "command": "make",
      "args": [],
      "problemMatcher": ["$gcc"],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    },
    {
      "label": "C51: Clean",
      "type": "shell",
      "command": "make",
      "args": ["clean"],
      "problemMatcher": []
    },
    {
      "label": "C51: Flash",
      "type": "shell",
      "command": "make",
      "args": ["flash", "PORT=/dev/ttyUSB0"],
      "problemMatcher": []
    }
  ]
}
'''
    tasks_file = project_path / ".vscode" / "tasks.json"
    tasks_file.write_text(tasks_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {tasks_file}", Colors.GREEN)

def create_c_cpp_properties_json(project_path):
    """创建 c_cpp_properties.json"""
    cpp_properties_content = '''{
  "configurations": [
    {
      "name": "SDCC C51",
      "includePath": [
        "${workspaceFolder}/**",
        "${workspaceFolder}/include",
        "${workspaceFolder}/src",
        "/usr/share/sdcc/include/mcs51",
        "/usr/share/sdcc/include"
      ],
      "defines": [
        "__SDCC__",
        "__SDCC_mcs51__",
        "__SDCC_MODEL_SMALL__"
      ],
      "compilerPath": "/usr/bin/sdcc",
      "cStandard": "c99",
      "intelliSenseMode": "gcc-x86",
      "compilerArgs": [
        "-mmcs51",
        "--model-small",
        "--std-c99"
      ],
      "browse": {
        "path": [
          "${workspaceFolder}/**",
          "/usr/share/sdcc/include"
        ],
        "limitSymbolsToIncludedHeaders": false
      }
    }
  ],
  "version": 4
}
'''
    cpp_properties_file = project_path / ".vscode" / "c_cpp_properties.json"
    cpp_properties_file.write_text(cpp_properties_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {cpp_properties_file}", Colors.GREEN)

def create_settings_json(project_path):
    """创建 settings.json"""
    settings_content = '''{
  "files.associations": {
    "*.h": "c",
    "8051.h": "c",
    "8052.h": "c",
    "reg52.h": "c"
  },
  "C_Cpp.errorSquiggles": "disabled",
  "C_Cpp.intelliSenseEngine": "default",
  "C_Cpp.default.includePath": [
    "${workspaceFolder}/include",
    "${workspaceFolder}/src"
  ],
  "C_Cpp.default.defines": [
    "__SDCC__",
    "__SDCC_mcs51__"
  ]
}
'''
    settings_file = project_path / ".vscode" / "settings.json"
    settings_file.write_text(settings_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {settings_file}", Colors.GREEN)

def create_main_c(project_path):
    """创建示例 main.c"""
    main_c_content = '''#include <8052.h>  // STC89C52 对应 8052.h
#include "delay.h"    // 延时函数

// SDCC 定义位引脚的语法
// P1.0 引脚 (P1 寄存器地址 0x90)
__sbit __at (0x90) LED; 

/**
 * 主函数
 */
void main() {
    // 初始化
    LED = 1;  // 初始状态：LED 熄灭
    
    // 主循环
    while (1) {
        LED = 0;  // 低电平点亮 (大部分开发板)
        delay(500);
        LED = 1;  // 高电平熄灭
        delay(500);
    }
}
'''
    main_c_file = project_path / "src" / "main.c"
    main_c_file.write_text(main_c_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {main_c_file}", Colors.GREEN)

def create_delay_h(project_path):
    """创建 delay.h 头文件"""
    delay_h_content = '''#ifndef DELAY_H
#define DELAY_H

/**
 * 延时函数
 * @param ms 延时毫秒数（近似值）
 * 
 * 注意：此延时函数基于循环计数，实际延时时间取决于
 * 单片机的工作频率。对于 11.0592MHz 的晶振，此函数
 * 提供近似的毫秒级延时。
 */
void delay(unsigned int ms);

/**
 * 微秒级延时（近似值）
 * @param us 延时微秒数
 * 
 * 注意：此函数精度较低，仅适用于粗略延时
 */
void delay_us(unsigned int us);

#endif // DELAY_H
'''
    delay_h_file = project_path / "include" / "delay.h"
    delay_h_file.write_text(delay_h_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {delay_h_file}", Colors.GREEN)

def create_delay_c(project_path):
    """创建 delay.c 源文件"""
    delay_c_content = '''#include "delay.h"

/**
 * 延时函数（毫秒级）
 */
void delay(unsigned int ms) {
    unsigned int i, j;
    // 根据晶振频率调整循环次数
    // 对于 11.0592MHz: 约 120 次循环 ≈ 1ms
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/**
 * 延时函数（微秒级）
 */
void delay_us(unsigned int us) {
    unsigned int i;
    // 粗略的微秒级延时
    // 对于 11.0592MHz: 约 1 次循环 ≈ 1us
    for (i = 0; i < us; i++);
}
'''
    delay_c_file = project_path / "src" / "delay.c"
    delay_c_file.write_text(delay_c_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {delay_c_file}", Colors.GREEN)

def create_sdcc_compat_h(project_path):
    """创建 SDCC 兼容性头文件（用于 IntelliSense）"""
    compat_h_content = '''#ifndef SDCC_COMPAT_H
#define SDCC_COMPAT_H

/**
 * SDCC 兼容性头文件
 * 
 * 此文件仅用于 VSCode IntelliSense，不影响实际编译。
 * SDCC 编译器会忽略这些定义，使用其内置的 __sbit 和 __at 语法。
 * 
 * 如果 IntelliSense 仍然报错，可以在 c_cpp_properties.json 中
 * 设置 "C_Cpp.errorSquiggles": "disabled" 来禁用错误检查。
 */

// 为了 IntelliSense 兼容性，定义 __sbit 和 __at
// 实际编译时，SDCC 会使用其内置定义，这些会被忽略
#ifndef __SDCC__
// 仅在非 SDCC 环境下定义（用于 IntelliSense）
#define __sbit volatile unsigned char
#define __at(x) 
#endif

#endif // SDCC_COMPAT_H
'''
    compat_h_file = project_path / "include" / "sdcc_compat.h"
    compat_h_file.write_text(compat_h_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {compat_h_file}", Colors.GREEN)

def create_config_h(project_path):
    """创建 config.h 配置文件"""
    config_h_content = '''#ifndef CONFIG_H
#define CONFIG_H

// 包含 SDCC 兼容性定义（用于 IntelliSense）
#include "sdcc_compat.h"

// 晶振频率 (Hz)
#define FOSC 11059200UL

// 系统时钟周期 (秒)
#define T_CLK (1.0 / FOSC)

// 机器周期 (12 个时钟周期)
#define T_MACHINE (12.0 * T_CLK)

// 常用引脚定义
// P1 口
__sbit __at (0x90) P1_0;
__sbit __at (0x91) P1_1;
__sbit __at (0x92) P1_2;
__sbit __at (0x93) P1_3;
__sbit __at (0x94) P1_4;
__sbit __at (0x95) P1_5;
__sbit __at (0x96) P1_6;
__sbit __at (0x97) P1_7;

// P2 口
__sbit __at (0xA0) P2_0;
__sbit __at (0xA1) P2_1;
__sbit __at (0xA2) P2_2;
__sbit __at (0xA3) P2_3;
__sbit __at (0xA4) P2_4;
__sbit __at (0xA5) P2_5;
__sbit __at (0xA6) P2_6;
__sbit __at (0xA7) P2_7;

// P3 口（特殊功能）
__sbit __at (0xB0) P3_0;  // RXD
__sbit __at (0xB1) P3_1;  // TXD
__sbit __at (0xB2) P3_2;  // INT0
__sbit __at (0xB3) P3_3;  // INT1
__sbit __at (0xB4) P3_4;  // T0
__sbit __at (0xB5) P3_5;  // T1
__sbit __at (0xB6) P3_6;  // WR
__sbit __at (0xB7) P3_7;  // RD

#endif // CONFIG_H
'''
    config_h_file = project_path / "include" / "config.h"
    config_h_file.write_text(config_h_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {config_h_file}", Colors.GREEN)

def create_gitignore(project_path):
    """创建 .gitignore"""
    gitignore_content = '''# 构建输出文件
build/
*.ihx
*.lk
*.lst
*.map
*.mem
*.rel
*.rst
*.sym
*.asm
*.adb
*.cdb
*.omf

# SDCC 生成的文件
*.lnk
*.noi

# 临时文件
*~
*.swp
*.swo
.DS_Store
Thumbs.db

# IDE
.vscode/settings.json
.idea/
*.suo
*.user
*.userosscache
*.sln.docstates
'''
    gitignore_file = project_path / ".gitignore"
    gitignore_file.write_text(gitignore_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {gitignore_file}", Colors.GREEN)

def create_readme(project_path, project_name):
    """创建 README.md"""
    readme_content = f'''# {project_name}

STC89C52RC C51 单片机项目

## 项目结构

```
{project_name}/
├── .vscode/              # VSCode 配置文件
│   ├── tasks.json        # 构建任务配置
│   ├── c_cpp_properties.json  # IntelliSense 配置
│   └── settings.json     # 工作区设置
├── build/                # 构建输出目录
├── include/              # 头文件目录
│   ├── config.h         # 配置文件
│   └── delay.h          # 延时函数声明
├── lib/                  # 库文件目录
├── src/                  # 源代码目录
│   ├── main.c           # 主程序
│   └── delay.c          # 延时函数实现
├── Makefile             # Make 构建配置
├── build.sh             # 构建和烧录脚本
├── .gitignore
└── README.md
```

## 前置要求

- **SDCC 编译器**: Small Device C Compiler (用于编译 C51 代码)
- **stcgal**: STC 单片机烧录工具
- **串口工具**: 用于连接单片机（如 USB 转串口模块）

### 安装 SDCC (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install sdcc
```

### 安装 stcgal

```bash
# 使用 pip 安装
pip install stcgal

# 或从源码安装
git clone https://github.com/grigori/stcgal.git
cd stcgal
sudo python3 setup.py install
```

### 检查串口设备

```bash
# 查看可用串口
ls -l /dev/ttyUSB* /dev/ttyACM*

# 添加用户到 dialout 组（避免权限问题）
sudo usermod -a -G dialout $USER
# 然后重新登录
```

## 使用方法

### 方式一：使用 Embedded IDE 插件（推荐）✨

[Embedded IDE](https://marketplace.visualstudio.com/items?itemName=CL.eide) 是一个强大的 MCU 开发插件，支持 8051/STM8/Cortex-M 等平台。

#### 优势：
- ✅ **自动解决 IntelliSense 错误**：内置 C/C++ IntelliSense Provider，自动识别 SDCC 特殊语法（`__sbit`, `__at` 等），无需手动配置
- ✅ **集成构建和烧录**：支持 SDCC 工具链，提供图形化构建和烧录界面
- ✅ **项目模板**：提供多种项目模板快速开始
- ✅ **调试支持**：自动生成调试配置
- ✅ **串口监视器**：内置串口监视功能

#### 使用步骤：

1. **安装插件**：
   - 在 VSCode 扩展市场搜索 "Embedded IDE" 并安装
   - 或访问：https://marketplace.visualstudio.com/items?itemName=CL.eide

2. **配置编译器路径**：
   - 打开 VSCode 操作栏（侧边栏的 Embedded IDE 图标）
   - 设置 SDCC 编译器路径（通常为 `/usr/bin/sdcc`）

3. **导入或创建项目**：
   - 可以使用插件导入现有项目
   - 或使用插件创建新的 8051 项目模板

4. **构建和烧录**：
   - 使用插件提供的构建按钮
   - 配置烧录工具（stcgal）进行烧录

#### 注意事项：
- 使用 Embedded IDE 插件时，可以忽略 `c_cpp_properties.json` 中的 IntelliSense 错误
- 插件会自动处理 SDCC 的特殊语法识别
- 如果同时使用插件和手动构建，两者可以共存

### 方式二：使用 VSCode 任务（传统方式）

1. 打开项目文件夹
2. 按 `Ctrl+Shift+B` 构建项目
3. 使用任务 "C51: Flash" 烧录到单片机（需要先修改串口设备）

**注意**：如果使用传统方式，IntelliSense 可能会显示 `__sbit` 和 `__at` 的错误（红色波浪线），这是正常的，不影响实际编译。可以通过以下方式解决：
- 安装 Embedded IDE 插件（推荐）
- 或在 `settings.json` 中设置 `"C_Cpp.errorSquiggles": "disabled"` 来禁用错误检查

### 使用命令行

#### 使用 Makefile

```bash
# 编译项目
make

# 清理构建文件
make clean

# 烧录到单片机
make flash PORT=/dev/ttyUSB0

# 查看帮助
make help
```

#### 使用 build.sh 脚本

```bash
# 编译项目
./build.sh

# 编译并烧录（交互式）
./build.sh -p /dev/ttyUSB0

# 清理构建文件
./build.sh -c

# 查看帮助
./build.sh -h
```

#### 手动编译和烧录

```bash
# 1. 编译
mkdir -p build
sdcc -mmcs51 --model-small --std-c99 --opt-code-size --nooverlay \\
     -Iinclude -Isrc \\
     -o build/ src/*.c

# 2. 烧录
stcgal -p /dev/ttyUSB0 -P stc89 build/{project_name}.ihx
```

## 开发说明

### 添加新的源文件

1. 在 `src/` 目录创建新的 `.c` 文件
2. Makefile 会自动包含所有 `src/*.c` 文件
3. 如果需要在头文件中声明，在 `include/` 目录创建对应的 `.h` 文件

### 使用延时函数

项目已包含延时函数库：

```c
#include "delay.h"

void main() {{
    delay(1000);      // 延时 1000 毫秒
    delay_us(500);    // 延时 500 微秒
}}
```

### 引脚定义

在 `include/config.h` 中已定义常用引脚：

```c
#include "config.h"

void main() {{
    P1_0 = 0;  // P1.0 输出低电平
    P2_3 = 1;  // P2.3 输出高电平
}}
```

### SDCC 编译器选项说明

- `-mmcs51`: 指定目标架构为 MCS-51（8051 系列）
- `--model-small`: 使用小内存模型（128 字节内部 RAM）
- `--std-c99`: 使用 C99 标准
- `--opt-code-size`: 优化代码大小
- `--nooverlay`: 不使用覆盖段（避免函数参数覆盖）

### 内存模型选择

- `--model-small`: 小模型（128 字节内部 RAM，默认）
- `--model-medium`: 中模型（256 字节内部 RAM）
- `--model-large`: 大模型（512+ 字节内部 RAM）

根据你的单片机型号选择合适的模型。

## STC89C52RC 规格

- **Flash 程序存储器**: 8KB
- **内部 RAM**: 512 字节
- **EEPROM**: 6KB
- **工作频率**: 0-40MHz（典型 11.0592MHz）
- **I/O 口**: 32 个（P0, P1, P2, P3）

## 常见问题

### 1. 编译错误：找不到头文件

确保 SDCC 已正确安装：
```bash
which sdcc
sdcc --version
```

检查头文件路径：
```bash
ls /usr/share/sdcc/include/mcs51/
```

### 2. 烧录失败：权限 denied

```bash
# 添加用户到 dialout 组
sudo usermod -a -G dialout $USER
# 重新登录后生效
```

或使用 sudo（不推荐）：
```bash
sudo stcgal -p /dev/ttyUSB0 -P stc89 build/{project_name}.ihx
```

### 3. 烧录失败：找不到设备

```bash
# 检查串口设备
ls -l /dev/ttyUSB* /dev/ttyACM*

# 检查设备权限
groups  # 确认是否在 dialout 组中

# 检查设备是否被占用
sudo lsof /dev/ttyUSB0
```

### 4. 延时函数不准确

延时函数的精度取决于：
- 晶振频率（默认配置为 11.0592MHz）
- 编译器优化选项
- 实际硬件差异

如需精确延时，建议使用定时器中断。

### 5. IntelliSense 报错（`__sbit`, `__at` 未定义）

**问题**：VSCode 的 IntelliSense 可能无法识别 SDCC 的特殊语法（如 `__sbit`, `__at`），显示红色错误。

**解决方案（推荐）**：
- 安装 **Embedded IDE** 插件，它会自动处理 SDCC 语法识别，无需额外配置

**其他解决方案**：
- 在 `settings.json` 中禁用错误检查：
```json
{{
  "C_Cpp.errorSquiggles": "disabled"
}}
```
- 或使用项目中的 `include/sdcc_compat.h` 兼容性头文件（仅用于 IntelliSense）

**注意**：这些错误不影响实际编译，SDCC 编译器能正确识别这些语法。

## 调试技巧

### 1. 查看编译生成的汇编代码

编译后会生成 `.asm` 文件，可以查看编译器生成的汇编代码：
```bash
cat build/*.asm
```

### 2. 查看内存映射

编译后会生成 `.map` 文件，包含内存使用情况：
```bash
cat build/*.map
```

### 3. 使用串口调试

可以通过串口输出调试信息（需要配置串口和波特率）：
```c
// 示例：通过串口发送数据
// 需要配置定时器和串口相关寄存器
```

## 参考资源

- [SDCC 官方文档](http://sdcc.sourceforge.net/doc/)
- [STC89C52RC 数据手册](http://www.stcmcudata.com/)
- [stcgal 项目](https://github.com/grigori/stcgal)
- [Embedded IDE 插件](https://marketplace.visualstudio.com/items?itemName=CL.eide) - 推荐使用，解决 IntelliSense 问题
- [Embedded IDE 文档](https://em-ide.com)

## 许可证

本项目模板遵循 MIT 许可证。
'''
    readme_file = project_path / "README.md"
    readme_file.write_text(readme_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {readme_file}", Colors.GREEN)

def main():
    print_colored("=" * 40, Colors.BLUE)
    print_colored("  C51 单片机项目创建工具", Colors.BLUE)
    print_colored("=" * 40, Colors.BLUE)
    print()
    
    # 获取项目名称
    if len(sys.argv) > 1:
        project_name = sys.argv[1]
    else:
        project_name = input("请输入项目名称: ").strip()
    
    if not project_name:
        print_colored("错误: 项目名称不能为空", Colors.RED)
        sys.exit(1)
    
    print()
    print_colored("正在创建项目结构...", Colors.BLUE)
    print()
    
    # 创建项目结构
    project_path = create_project_structure(project_name)
    
    # 创建构建配置文件
    create_makefile(project_path, project_name)
    create_build_sh(project_path, project_name)
    
    # 创建 VSCode 配置文件
    create_tasks_json(project_path)
    create_c_cpp_properties_json(project_path)
    create_settings_json(project_path)
    
    # 创建示例文件
    create_sdcc_compat_h(project_path)
    create_main_c(project_path)
    create_delay_h(project_path)
    create_delay_c(project_path)
    create_config_h(project_path)
    create_gitignore(project_path)
    create_readme(project_path, project_name)
    
    print()
    print_colored(f"✓ 项目 '{project_name}' 创建成功!", Colors.GREEN)
    print()
    print_colored("项目结构:", Colors.BLUE)
    
    # 显示项目结构
    for root, dirs, files in os.walk(project_path):
        level = root.replace(str(project_path), '').count(os.sep)
        indent = ' ' * 2 * level
        print(f"{indent}{os.path.basename(root)}/")
        subindent = ' ' * 2 * (level + 1)
        for file in files:
            print(f"{subindent}{file}")
    
    print()
    print_colored("下一步:", Colors.YELLOW)
    print(f"  1. cd {project_name}")
    print("  2. code .")
    print("  3. 选择构建方式:")
    print("     - 推荐: 安装 Embedded IDE 插件（自动解决 IntelliSense 错误）")
    print("     - 或: 按 Ctrl+Shift+B 使用 VSCode 任务构建")
    print("     - 或: 使用 make 命令构建")
    print("  4. 烧录到单片机: make flash PORT=/dev/ttyUSB0")
    print()
    print_colored("提示:", Colors.YELLOW)
    print("  - 确保已安装 SDCC: sudo apt-get install sdcc")
    print("  - 确保已安装 stcgal: pip install stcgal")
    print("  - 检查串口设备: ls -l /dev/ttyUSB*")
    print("  - 添加用户到 dialout 组: sudo usermod -a -G dialout $USER")
    print()
    print_colored("关于 IntelliSense 错误:", Colors.YELLOW)
    print("  - 如果看到 __sbit 或 __at 的红色错误，这是正常的")
    print("  - 推荐安装 Embedded IDE 插件自动解决")
    print("  - 或查看 README.md 中的解决方案")
    print()

if __name__ == "__main__":
    main()