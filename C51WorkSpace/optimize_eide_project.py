#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Embedded IDE 项目优化脚本

功能：
1. 检查并修复硬编码的 COM 端口配置（如 COM3）
2. 自动检测系统上可用的串口设备
3. 更新 .eide/eide.yml 配置文件
4. 验证 stcflash.py 是否包含自动检测功能
5. 提供详细的错误提示和建议
"""

import os
import sys
import re
import glob
import yaml
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

def find_serial_ports():
    """自动检测可用的串口设备"""
    ports = []
    if sys.platform == "win32":
        # Windows: COM1-COM256
        for i in range(1, 257):
            port = "COM%d" % i
            try:
                # 尝试打开端口来检测是否存在
                import serial
                try:
                    ser = serial.Serial(port, timeout=0.1)
                    ser.close()
                    ports.append(port)
                except (serial.SerialException, OSError):
                    pass
            except ImportError:
                # 如果没有 pyserial，使用文件系统检查
                if os.path.exists(port):
                    ports.append(port)
    elif sys.platform == "darwin":
        # macOS: /dev/tty.* 和 /dev/cu.*
        ports = glob.glob("/dev/tty.*") + glob.glob("/dev/cu.*")
        ports = [p for p in ports if "usbserial" in p or "USB" in p or "usbmodem" in p]
        ports.sort()
    else:
        # Linux: /dev/ttyUSB* 和 /dev/ttyACM*
        ports = glob.glob("/dev/ttyUSB*") + glob.glob("/dev/ttyACM*")
        ports.sort()
    return ports

def map_com_port_to_linux(com_port):
    """将 Windows COM 端口名称映射到 Linux 设备路径"""
    if com_port.upper().startswith("COM"):
        try:
            com_num = int(com_port[3:])
            # COM3 -> /dev/ttyUSB0 (COM3 对应索引 2, 即 ttyUSB0)
            usb_index = com_num - 1
            candidates = [
                "/dev/ttyUSB%d" % usb_index,
                "/dev/ttyACM%d" % usb_index,
            ]
            # 检查候选端口是否存在
            for candidate in candidates:
                if os.path.exists(candidate):
                    return candidate
            # 如果都不存在，返回第一个候选（用户可能需要插入设备）
            return candidates[0]
        except ValueError:
            pass
    return None

def check_stcflash_has_autodetect():
    """检查 stcflash.py 是否包含自动检测功能"""
    stcflash_path = Path("tools/stcflash.py")
    if not stcflash_path.exists():
        return False, "stcflash.py 不存在"
    
    try:
        content = stcflash_path.read_text(encoding='utf-8')
        has_find_serial = "def find_serial_ports" in content
        has_map_com = "def map_com_port" in content
        has_autodetect = "自动检测" in content or "自动使用" in content
        
        if has_find_serial and has_map_com and has_autodetect:
            return True, "已包含自动检测功能"
        else:
            missing = []
            if not has_find_serial:
                missing.append("find_serial_ports()")
            if not has_map_com:
                missing.append("map_com_port()")
            if not has_autodetect:
                missing.append("自动检测逻辑")
            return False, f"缺少: {', '.join(missing)}"
    except Exception as e:
        return False, f"读取文件失败: {str(e)}"

def optimize_eide_config(project_path=None):
    """优化 .eide/eide.yml 配置文件"""
    if project_path is None:
        project_path = Path.cwd()
    else:
        project_path = Path(project_path)
    
    eide_config_path = project_path / ".eide" / "eide.yml"
    
    if not eide_config_path.exists():
        print_colored(f"错误: 找不到配置文件 {eide_config_path}", Colors.RED)
        return False
    
    print_colored(f"正在检查配置文件: {eide_config_path}", Colors.BLUE)
    
    try:
        # 读取 YAML 文件
        with open(eide_config_path, 'r', encoding='utf-8') as f:
            content = f.read()
            config = yaml.safe_load(content)
    except Exception as e:
        print_colored(f"错误: 无法读取配置文件: {str(e)}", Colors.RED)
        return False
    
    # 检查是否有硬编码的 COM 端口
    needs_update = False
    changes = []
    
    # 查找所有目标配置
    if 'targets' in config:
        for target_name, target_config in config['targets'].items():
            if 'uploadConfigMap' in target_config:
                for uploader_name, uploader_config in target_config['uploadConfigMap'].items():
                    if 'commandLine' in uploader_config:
                        cmd_line = uploader_config['commandLine']
                        
                        # 检查是否包含 COM 端口（Windows 格式）
                        com_match = re.search(r'-p\s+(COM\d+)', cmd_line)
                        if com_match:
                            com_port = com_match.group(1)
                            print_colored(f"发现硬编码的 COM 端口: {com_port}", Colors.YELLOW)
                            
                            # 检测可用串口
                            available_ports = find_serial_ports()
                            
                            if sys.platform == "win32":
                                # Windows: 保持 COM 端口格式
                                if com_port in available_ports:
                                    print_colored(f"  COM 端口 {com_port} 可用", Colors.GREEN)
                                else:
                                    if available_ports:
                                        new_port = available_ports[0]
                                        print_colored(f"  COM 端口 {com_port} 不可用，建议使用: {new_port}", Colors.YELLOW)
                                    else:
                                        print_colored(f"  未找到可用串口", Colors.RED)
                            else:
                                # Linux/macOS: 映射到设备路径
                                mapped_port = map_com_port_to_linux(com_port)
                                if mapped_port and os.path.exists(mapped_port):
                                    new_cmd = re.sub(r'-p\s+COM\d+', f'-p {mapped_port}', cmd_line)
                                    uploader_config['commandLine'] = new_cmd
                                    needs_update = True
                                    changes.append(f"{target_name}/{uploader_name}: {com_port} -> {mapped_port}")
                                    print_colored(f"  映射到: {mapped_port}", Colors.GREEN)
                                elif available_ports:
                                    # 使用第一个可用端口
                                    new_port = available_ports[0]
                                    new_cmd = re.sub(r'-p\s+COM\d+', f'-p {new_port}', cmd_line)
                                    uploader_config['commandLine'] = new_cmd
                                    needs_update = True
                                    changes.append(f"{target_name}/{uploader_name}: {com_port} -> {new_port}")
                                    print_colored(f"  自动使用可用端口: {new_port}", Colors.GREEN)
                                else:
                                    # 使用映射的端口（即使不存在，stcflash.py 会自动检测）
                                    if mapped_port:
                                        new_cmd = re.sub(r'-p\s+COM\d+', f'-p {mapped_port}', cmd_line)
                                        uploader_config['commandLine'] = new_cmd
                                        needs_update = True
                                        changes.append(f"{target_name}/{uploader_name}: {com_port} -> {mapped_port} (自动检测)")
                                        print_colored(f"  映射到: {mapped_port} (设备未连接，将自动检测)", Colors.YELLOW)
                        
                        # 检查是否使用 /dev/ttyUSB0 但设备不存在
                        tty_match = re.search(r'-p\s+(/dev/tty\w+)', cmd_line)
                        if tty_match and sys.platform != "win32":
                            tty_port = tty_match.group(1)
                            if not os.path.exists(tty_port):
                                available_ports = find_serial_ports()
                                if available_ports:
                                    print_colored(f"  端口 {tty_port} 不存在，建议使用: {available_ports[0]}", Colors.YELLOW)
                                else:
                                    print_colored(f"  端口 {tty_port} 不存在，且未找到可用串口", Colors.RED)
    
    # 如果有更改，保存文件
    if needs_update:
        try:
            with open(eide_config_path, 'w', encoding='utf-8') as f:
                yaml.dump(config, f, allow_unicode=True, default_flow_style=False, sort_keys=False)
            print_colored(f"\n✓ 配置文件已更新", Colors.GREEN)
            print_colored("更改内容:", Colors.BLUE)
            for change in changes:
                print_colored(f"  - {change}", Colors.GREEN)
            return True
        except Exception as e:
            print_colored(f"错误: 无法保存配置文件: {str(e)}", Colors.RED)
            return False
    else:
        print_colored("✓ 配置文件无需更新", Colors.GREEN)
        return True

def show_serial_port_info():
    """显示串口设备信息"""
    print_colored("\n串口设备信息:", Colors.BLUE)
    available_ports = find_serial_ports()
    
    if available_ports:
        print_colored(f"找到 {len(available_ports)} 个可用串口:", Colors.GREEN)
        for i, port in enumerate(available_ports, 1):
            exists = os.path.exists(port) if not port.startswith("COM") else True
            status = "✓" if exists else "✗"
            print_colored(f"  {i}. {status} {port}", Colors.GREEN if exists else Colors.YELLOW)
    else:
        print_colored("未找到可用串口设备", Colors.YELLOW)
        print_colored("提示:", Colors.YELLOW)
        if sys.platform == "win32":
            print_colored("  - 在 Windows 上，请检查设备管理器中的 COM 端口", Colors.YELLOW)
        elif sys.platform == "darwin":
            print_colored("  - 在 macOS 上，请检查 /dev/tty.* 和 /dev/cu.*", Colors.YELLOW)
        else:
            print_colored("  - 在 Linux 上，请检查 /dev/ttyUSB* 和 /dev/ttyACM*", Colors.YELLOW)
            print_colored("  - 确保 USB 转串口设备已连接", Colors.YELLOW)
            print_colored("  - 检查用户是否在 dialout 组中: groups $USER", Colors.YELLOW)

def main():
    print_colored("=" * 60, Colors.BLUE)
    print_colored("  Embedded IDE 项目优化工具", Colors.BLUE)
    print_colored("=" * 60, Colors.BLUE)
    print()
    
    # 获取项目路径
    if len(sys.argv) > 1:
        project_path = sys.argv[1]
    else:
        project_path = None
    
    # 显示系统信息
    print_colored(f"操作系统: {sys.platform}", Colors.BLUE)
    print_colored(f"工作目录: {Path.cwd()}", Colors.BLUE)
    if project_path:
        print_colored(f"项目路径: {project_path}", Colors.BLUE)
    print()
    
    # 检查 stcflash.py
    print_colored("检查 stcflash.py...", Colors.BLUE)
    has_autodetect, message = check_stcflash_has_autodetect()
    if has_autodetect:
        print_colored(f"  ✓ {message}", Colors.GREEN)
    else:
        print_colored(f"  ⚠ {message}", Colors.YELLOW)
        print_colored("  建议: 确保 stcflash.py 包含自动检测功能", Colors.YELLOW)
    print()
    
    # 显示串口信息
    show_serial_port_info()
    print()
    
    # 优化配置文件
    print_colored("优化配置文件...", Colors.BLUE)
    success = optimize_eide_config(project_path)
    print()
    
    # 总结
    if success:
        print_colored("=" * 60, Colors.GREEN)
        print_colored("  优化完成！", Colors.GREEN)
        print_colored("=" * 60, Colors.GREEN)
        print()
        print_colored("下一步:", Colors.YELLOW)
        print_colored("  1. 检查 .eide/eide.yml 中的串口配置", Colors.YELLOW)
        print_colored("  2. 如果串口设备未连接，stcflash.py 会自动检测", Colors.YELLOW)
        print_colored("  3. 尝试烧录程序验证配置", Colors.YELLOW)
    else:
        print_colored("=" * 60, Colors.RED)
        print_colored("  优化过程中出现错误", Colors.RED)
        print_colored("=" * 60, Colors.RED)
        sys.exit(1)

if __name__ == "__main__":
    # 检查是否安装了 PyYAML
    try:
        import yaml
    except ImportError:
        print_colored("错误: 需要安装 PyYAML 库", Colors.RED)
        print_colored("安装命令: pip install PyYAML", Colors.YELLOW)
        sys.exit(1)
    
    main()
