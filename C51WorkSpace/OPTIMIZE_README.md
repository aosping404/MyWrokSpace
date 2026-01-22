# Embedded IDE 项目优化脚本使用说明

## 简介

`optimize_eide_project.py` 是一个用于优化 Embedded IDE 生成的项目配置的脚本。主要功能包括：

1. ✅ **自动检测并修复硬编码的 COM 端口配置**（如 COM3）
2. ✅ **自动检测系统上可用的串口设备**
3. ✅ **更新 .eide/eide.yml 配置文件**
4. ✅ **验证 stcflash.py 是否包含自动检测功能**
5. ✅ **提供详细的错误提示和建议**

## 前置要求

### 必需依赖

```bash
# 安装 PyYAML（用于解析和修改 YAML 配置文件）
pip install PyYAML

# 或者使用 pip3
pip3 install PyYAML
```

### 可选依赖

如果系统上安装了 `pyserial`，脚本可以更准确地检测 Windows 上的 COM 端口：

```bash
pip install pyserial
```

## 使用方法

### 基本用法

在项目根目录下运行：

```bash
# 方式一：直接运行（自动检测当前目录）
python3 optimize_eide_project.py

# 方式二：指定项目路径
python3 optimize_eide_project.py /path/to/project

# 方式三：使用执行权限（如果已添加）
./optimize_eide_project.py
```

### 使用示例

#### 示例 1：优化当前项目

```bash
cd mcs51_sdcc-pro01
python3 ../optimize_eide_project.py
```

输出示例：
```
============================================================
  Embedded IDE 项目优化工具
============================================================

操作系统: linux
工作目录: /home/user/mcs51_sdcc-pro01

检查 stcflash.py...
  ✓ 已包含自动检测功能

串口设备信息:
找到 1 个可用串口:
  1. ✓ /dev/ttyUSB0

优化配置文件...
正在检查配置文件: .eide/eide.yml
发现硬编码的 COM 端口: COM3
  映射到: /dev/ttyUSB0
✓ 配置文件已更新

更改内容:
  - Debug/Custom: COM3 -> /dev/ttyUSB0

============================================================
  优化完成！
============================================================
```

#### 示例 2：自动检测串口

如果配置文件中硬编码了不存在的端口，脚本会自动检测并使用第一个可用端口：

```bash
python3 optimize_eide_project.py
```

如果 `/dev/ttyUSB0` 不存在，脚本会：
1. 检测所有可用串口
2. 自动使用第一个可用端口
3. 更新配置文件

## 功能说明

### 1. COM 端口映射

脚本会自动将 Windows 格式的 COM 端口名称映射到 Linux/macOS 设备路径：

- `COM3` → `/dev/ttyUSB0` (如果存在)
- `COM4` → `/dev/ttyUSB1` (如果存在)
- 如果映射的设备不存在，会尝试使用第一个可用端口

### 2. 自动串口检测

脚本支持多平台串口检测：

- **Linux**: 检测 `/dev/ttyUSB*` 和 `/dev/ttyACM*`
- **macOS**: 检测 `/dev/tty.*` 和 `/dev/cu.*` (USB 串口)
- **Windows**: 检测 `COM1-COM256`

### 3. stcflash.py 验证

脚本会检查 `tools/stcflash.py` 是否包含以下功能：
- `find_serial_ports()` 函数
- `map_com_port()` 函数
- 自动检测逻辑

### 4. 配置文件更新

脚本会：
- 读取 `.eide/eide.yml` 配置文件
- 查找所有目标配置中的 `commandLine` 字段
- 检测硬编码的 COM 端口
- 自动更新为正确的设备路径
- 保留 YAML 文件格式和注释

## 常见问题

### Q1: 脚本提示 "需要安装 PyYAML 库"

**解决方案**：
```bash
pip install PyYAML
# 或
pip3 install PyYAML
```

### Q2: 脚本无法检测到串口设备

**可能原因**：
1. USB 转串口设备未连接
2. 设备驱动未安装
3. 权限问题（Linux）

**解决方案**：

**Linux**:
```bash
# 检查设备是否存在
ls -l /dev/ttyUSB* /dev/ttyACM*

# 检查用户是否在 dialout 组中
groups $USER

# 如果不在，添加到 dialout 组
sudo usermod -a -G dialout $USER
# 然后重新登录
```

**Windows**:
- 检查设备管理器中的 COM 端口
- 确保 USB 转串口驱动已安装

**macOS**:
```bash
# 检查设备
ls -l /dev/tty.* /dev/cu.*
```

### Q3: 配置文件更新后仍然无法烧录

**检查清单**：
1. ✅ 确认串口设备已连接
2. ✅ 检查 `.eide/eide.yml` 中的 `commandLine` 是否正确
3. ✅ 验证 `tools/stcflash.py` 是否包含自动检测功能
4. ✅ 尝试手动运行烧录命令：
   ```bash
   python ./tools/stcflash.py -p /dev/ttyUSB0 build/Debug/your_project.hex
   ```

### Q4: 脚本修改了配置文件，但我想恢复

**解决方案**：
- 使用版本控制（Git）恢复：
  ```bash
  git checkout .eide/eide.yml
  ```
- 或手动编辑 `.eide/eide.yml` 文件

## 工作原理

1. **读取配置**: 使用 PyYAML 解析 `.eide/eide.yml`
2. **检测串口**: 根据操作系统类型检测可用串口
3. **查找问题**: 使用正则表达式查找硬编码的 COM 端口
4. **映射端口**: 将 COM 端口名称映射到实际设备路径
5. **更新配置**: 更新 YAML 配置并保存

## 与 stcflash.py 的配合

`stcflash.py` 已经包含了自动检测功能：

- 如果指定的端口不存在，会自动检测并使用第一个可用端口
- 支持 COM 端口名称到 Linux 设备路径的映射
- 提供详细的错误提示和可用端口列表

优化脚本的作用是：
- **提前修复配置**，避免运行时错误
- **提供更好的用户体验**，自动选择正确的端口
- **跨平台兼容**，支持 Windows/Linux/macOS

## 贡献

如果发现问题或有改进建议，欢迎提交 Issue 或 Pull Request。

## 许可证

本脚本遵循 MIT 许可证。
