# 项目环境隔离脚本使用说明

## 概述

`project_env.sh` 是一个用于在不同项目间切换和隔离开发环境的脚本。它可以管理多个开发环境组件，避免环境冲突，确保不同项目使用正确的依赖和工具版本。

## 功能特性

### 支持的环境组件

1. **Conda环境** - Python包管理和虚拟环境（支持多个环境如yolo、base等）
2. **ROS2环境** - 机器人操作系统2
3. **MoveIt2环境** - 机器人运动规划框架
4. **HALCON环境** - 机器视觉软件库
5. **CUDA环境** - NVIDIA GPU计算平台
6. **Node.js环境** - JavaScript运行时和npm包管理器

### 预配置项目

- **yolo** - YOLOv8深度学习项目（已配置，包含PyTorch、CUDA支持）

### 核心功能

- ✅ **环境隔离**: 每个项目独立的环境配置
- ✅ **自动清理**: 切换环境时自动清理之前的环境变量
- ✅ **路径管理**: 自动管理PATH和LD_LIBRARY_PATH，去除重复项
- ✅ **配置管理**: 使用配置文件管理不同项目的环境设置
- ✅ **交互式选择**: 提供交互式菜单选择项目环境
- ✅ **状态显示**: 显示当前激活的环境组件

## 快速开始

### 1. 基本使用

```bash
# 交互式选择项目环境
source project_env.sh

# 直接切换到指定项目环境
source project_env.sh default

# 切换到自定义项目环境
source project_env.sh ros2_project
```

### 2. 查看帮助

```bash
source project_env.sh --help
```

### 3. 列出所有项目配置

```bash
source project_env.sh --list
```

### 4. 查看当前环境状态

```bash
source project_env.sh --show
```

## 配置文件管理

### 配置文件位置

所有项目配置文件保存在 `.project_env_configs/` 目录下，每个项目一个配置文件。

```
.project_env_configs/
├── default.conf          # 默认项目配置
├── ros2_project.conf     # ROS2项目配置
├── halcon_project.conf   # HALCON项目配置
└── ...
```

### 配置文件格式

配置文件使用bash脚本格式，每个配置项使用环境变量定义：

```bash
# 项目环境配置: project_name
# 生成时间: 2026-01-17 09:00:00

# Conda环境 (留空表示不使用Conda，或使用"base")
CONDA_ENV="base"

# ROS2配置 (留空表示不使用ROS2)
ROS_DISTRO="jazzy"

# MoveIt2配置 (留空表示不使用MoveIt2)
MOVEIT2_WS="$HOME/ws_moveit2"

# HALCON配置 (留空表示不使用HALCON)
HALCON_ROOT="/opt/halcon"

# CUDA配置 (留空表示不使用CUDA)
CUDA_VERSION="12.8"

# Node.js配置 (留空表示不使用Node.js)
NODE_VERSION="v20.19.5"
```

### 创建新项目配置

#### 方法1: 自动创建

```bash
# 切换到新项目，脚本会自动创建默认配置
source project_env.sh my_new_project
```

#### 方法2: 手动创建

```bash
# 1. 创建配置文件
cat > .project_env_configs/my_project.conf <<EOF
# 项目环境配置: my_project
CONDA_ENV="base"
ROS_DISTRO="jazzy"
MOVEIT2_WS="\$HOME/ws_moveit2"
CUDA_VERSION="12.8"
EOF

# 2. 切换到该项目
source project_env.sh my_project
```

## 使用示例

### 示例1: ROS2项目环境

创建ROS2项目配置文件 `.project_env_configs/ros2_project.conf`:

```bash
# ROS2项目配置
CONDA_ENV="base"
ROS_DISTRO="jazzy"
MOVEIT2_WS="$HOME/ws_moveit2"
CUDA_VERSION="12.8"
```

使用:

```bash
source project_env.sh ros2_project
```

### 示例2: HALCON视觉项目环境

创建HALCON项目配置文件 `.project_env_configs/halcon_project.conf`:

```bash
# HALCON项目配置
CONDA_ENV="base"
HALCON_ROOT="/opt/halcon"
CUDA_VERSION="12.8"
NODE_VERSION="v20.19.5"
```

使用:

```bash
source project_env.sh halcon_project
```

### 示例3: 纯Python项目环境

创建Python项目配置文件 `.project_env_configs/python_project.conf`:

```bash
# Python项目配置
CONDA_ENV="my_python_env"
NODE_VERSION="v20.19.5"
```

使用:

```bash
source project_env.sh python_project
```

### 示例4: YOLOv8 深度学习项目环境

YOLOv8项目已预配置，配置文件位于 `.project_env_configs/yolo.conf`:

```bash
# YOLOv8项目配置
CONDA_ENV="yolo"        # 使用专门的yolo Conda环境（包含PyTorch、torchvision等）
CUDA_VERSION="12.8"     # YOLOv8需要CUDA支持
# ROS2和MoveIt2均不激活，避免依赖冲突
```

使用:

```bash
# 切换到YOLO项目环境
source project_env.sh yolo

# 验证环境
python -c "import torch; print(f'PyTorch: {torch.__version__}, CUDA可用: {torch.cuda.is_available()}')"
```

**重要提示**: 
- YOLO环境与ROS2环境完全隔离，切换时自动清理ROS2相关环境变量
- 避免在YOLO环境中使用ROS2包，反之亦然
- 如果需要同时使用，请使用独立的终端窗口

### 示例5: 最小环境（无额外组件）

创建最小环境配置文件 `.project_env_configs/minimal.conf`:

```bash
# 最小环境配置
CONDA_ENV="base"
```

使用:

```bash
source project_env.sh minimal
```

## 命令行别名

脚本自动创建以下别名（在source后可用）：

- `penv` - 快捷方式：`source project_env.sh`
- `penv-list` - 列出所有项目配置
- `penv-show` - 显示当前环境状态

使用示例:

```bash
# 第一次使用后
source project_env.sh default

# 之后可以直接使用别名
penv ros2_project
penv-list
penv-show
```

## 工作原理

### 环境切换流程

1. **清理现有环境**: 自动清理当前激活的所有环境组件
2. **加载项目配置**: 从配置文件加载项目环境设置
3. **激活环境组件**: 按照配置激活所需的环境组件
4. **清理路径**: 去除PATH和LD_LIBRARY_PATH中的重复项
5. **显示状态**: 显示当前环境状态信息

### 环境清理机制

脚本在切换环境时会：

- 停用当前Conda环境（如果非base）
- 清理ROS2相关环境变量和路径
- 清理MoveIt2工作空间路径
- 清理HALCON环境变量和路径
- 清理CUDA路径和库路径
- 清理Node.js路径
- 去除PATH和LD_LIBRARY_PATH中的重复项

## 最佳实践

### 1. 为每个项目创建独立配置

```bash
# 为不同项目创建专门的配置
source project_env.sh yolo          # YOLOv8深度学习项目
source project_env.sh ros2_project  # ROS2机器人项目
source project_env.sh halcon_project # HALCON视觉项目
```

### 1.1 环境隔离建议

- **深度学习项目（如YOLO）**: 使用独立的Conda环境，不加载ROS2/MoveIt2
- **ROS2项目**: 使用base或专门的ROS2 Conda环境，加载ROS2和MoveIt2
- **混合项目**: 如果项目需要多个组件，谨慎配置，注意依赖冲突

**避免在同一个终端中同时激活冲突的环境**:
```bash
# ❌ 错误做法：在同一终端先后激活冲突环境
source project_env.sh yolo
source project_env.sh ros2_project  # 可能导致依赖冲突

# ✅ 正确做法：在独立终端中分别使用
# 终端1
source project_env.sh yolo
# 终端2
source project_env.sh ros2_project
```

### 2. 命名规范

建议使用描述性的项目名称：

- `ros2_navigation` - ROS2导航项目
- `halcon_inspection` - HALCON检测项目
- `python_ml` - Python机器学习项目
- `full_stack` - 全栈开发项目

### 3. 版本管理

配置文件可以纳入版本控制，但需要注意：

- 使用相对路径或环境变量（如 `$HOME`）
- 不包含敏感信息（API密钥等）
- 在团队中共享配置时，确保路径可移植

### 4. 定期检查环境状态

```bash
# 切换环境后检查状态
source project_env.sh my_project
penv-show
```

## 故障排除

### 问题1: Conda环境无法激活

**解决方法**:
- 确保Conda已正确安装
- 检查环境名称是否正确
- 使用 `conda env list` 查看可用环境

### 问题2: ROS2路径找不到

**解决方法**:
- 检查ROS2是否正确安装
- 确认 `ROS_DISTRO` 配置正确（如 "jazzy"）
- 验证 `/opt/ros/<distro>/` 路径存在

### 问题3: MoveIt2工作空间未编译

**解决方法**:
- 确保工作空间已编译：`cd ~/ws_moveit2 && colcon build`
- 检查 `install/` 目录是否存在
- 验证 `MOVEIT2_WS` 路径配置正确

### 问题4: 路径冲突

**解决方法**:
- 脚本会自动清理重复路径
- 如果仍有问题，检查 `.bashrc` 中的环境变量设置
- 使用 `penv-show` 查看当前环境状态

### 问题5: 配置文件不存在

**解决方法**:
- 脚本会自动创建默认配置
- 可以手动创建配置文件：`.project_env_configs/<project_name>.conf`
- 使用 `penv-list` 查看所有可用配置

## 高级用法

### 在Shell启动时自动切换

在 `~/.bashrc` 中添加：

```bash
# 自动切换到默认项目环境
if [[ -f "$HOME/yolov8/project_env.sh" ]]; then
    source "$HOME/yolov8/project_env.sh" default
fi
```

### 在项目目录中自动切换

创建项目根目录的 `.envrc` 文件（如果使用direnv）：

```bash
# .envrc
source_project_env() {
    source "$HOME/yolov8/project_env.sh" my_project
}
source_project_env
```

## 技术细节

### 支持的路径清理

- PATH环境变量去重
- LD_LIBRARY_PATH环境变量去重
- PYTHONPATH环境变量去重
- CMAKE_PREFIX_PATH环境变量管理

### 环境变量备份

脚本会备份关键环境变量，但主要用于清理，不提供恢复功能（因为每次切换都是完全清理）。

### 兼容性

- 支持bash 4.0+
- 兼容Ubuntu 24.04
- 与现有`.bashrc`配置兼容

## 更新日志

### v1.0.0 (2026-01-17)

- 初始版本
- 支持6种环境组件（Conda, ROS2, MoveIt2, HALCON, CUDA, Node.js）
- 交互式项目选择
- 配置文件管理
- 环境状态显示

## 贡献

如需改进脚本或添加新功能，请编辑 `project_env.sh` 并测试。

---

**提示**: 此脚本必须使用 `source` 命令执行，因为它需要修改当前shell的环境变量。
