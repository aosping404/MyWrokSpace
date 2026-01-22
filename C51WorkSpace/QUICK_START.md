# 快速开始 - Embedded IDE 项目优化

## 一键优化

在项目目录下运行：

```bash
python3 optimize_eide_project.py
```

## 功能

✅ 自动检测并修复硬编码的 COM 端口（如 COM3 → /dev/ttyUSB0）  
✅ 自动检测系统上可用的串口设备  
✅ 更新 `.eide/eide.yml` 配置文件  
✅ 验证 `stcflash.py` 是否包含自动检测功能  

## 安装依赖

```bash
pip install PyYAML
```

## 示例输出

```
============================================================
  Embedded IDE 项目优化工具
============================================================

操作系统: linux
检查 stcflash.py...
  ✓ 已包含自动检测功能

串口设备信息:
找到 1 个可用串口:
  1. ✓ /dev/ttyUSB0

优化配置文件...
发现硬编码的 COM 端口: COM3
  自动使用可用端口: /dev/ttyUSB0
✓ 配置文件已更新

更改内容:
  - Debug/Custom: COM3 -> /dev/ttyUSB0
```

## 详细文档

查看 [OPTIMIZE_README.md](OPTIMIZE_README.md) 获取完整文档。
