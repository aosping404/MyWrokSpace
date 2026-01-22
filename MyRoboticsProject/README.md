# MyRoboticsProject

ROS2 + OpenCV + YOLO26 + MoveIt2 融合项目工作区

## 概述

本工作区包含用于创建融合 OpenCV、YOLO26、ROS2 和 MoveIt2 的机器人视觉与运动规划项目的工具和配置。

## 环境配置

### ros2_yolo Conda 环境

已创建融合的 Conda 环境 `ros2_yolo`，包含：

- **Python**: 3.12.12 (兼容 ROS2 Jazzy)
- **PyTorch**: 2.9.1+cu128 (CUDA 12.8)
- **Ultralytics**: 8.4.5 (YOLO26)
- **OpenCV**: 4.12.0.88
- **ROS2 Python**: rclpy (通过系统安装)

### 环境切换

```bash
# 切换到 robotics 项目环境
cd ~/MyWorkSpace
source project_env.sh robotics

# 验证环境
penv-show
```

### 环境状态

✅ **Conda 环境**: ros2_yolo (已激活)
✅ **ROS2 环境**: jazzy (已激活)  
⚠️ **MoveIt2 环境**: 未激活（install 目录在非标准位置 `~/ws_moveit2/src/install/`）
✅ **CUDA 环境**: 12.8 (已激活)

## 工具和脚本

### 1. 创建 ROS2 项目脚本

`create_robotics_project.py` - 创建融合 OpenCV、YOLO26、ROS2、MoveIt2 的 ROS2 项目

```bash
python3 create_robotics_project.py my_project_name
```

### 2. 创建融合环境脚本

`create_ros2_yolo_env.sh` - 创建 ros2_yolo 融合 Conda 环境（已执行）

### 3. 测试环境脚本

`测试环境.sh` - 测试 robotics 环境配置

```bash
bash 测试环境.sh
```

## 项目配置文件

### robotics.conf

位置：`~/MyWorkSpace/.project_env_configs/robotics.conf`

配置内容：
```bash
CONDA_ENV="ros2_yolo"
ROS_DISTRO="jazzy"
MOVEIT2_WS="$HOME/ws_moveit2"
CUDA_VERSION="12.8"
```

## 使用示例

### 1. 创建新项目

```bash
# 切换到 robotics 环境
cd ~/MyWorkSpace
source project_env.sh robotics

# 创建新项目
cd MyRoboticsProject
python3 create_robotics_project.py my_robotics_project
```

### 2. 构建和运行项目

```bash
# 进入项目目录
cd my_robotics_project

# 构建项目
colcon build --packages-select my_robotics_project

# 激活构建环境
source install/setup.bash

# 运行项目
ros2 launch my_robotics_project my_robotics_project.launch.py
```

## 环境说明

### Python 环境

- **ros2_yolo**: Python 3.12.12，包含 PyTorch、YOLO26、OpenCV、ROS2 Python

### MoveIt2 工作空间

- **路径**: `~/ws_moveit2/`
- **install 目录**: `~/ws_moveit2/src/install/` (非标准位置)
- **注意**: 如果需要使用 MoveIt2，需要手动 source：
  ```bash
  source ~/ws_moveit2/src/install/setup.bash
  ```

## 文档

- [robotics环境配置说明.md](robotics环境配置说明.md) - 详细的环境配置说明
- [系统环境信息汇总.md](../系统环境信息汇总.md) - 完整的系统环境信息

## 故障排除

### MoveIt2 未激活

如果 MoveIt2 环境未自动激活，手动激活：

```bash
source ~/ws_moveit2/src/install/setup.bash
```

或者更新 `project_env.sh` 脚本以支持非标准的 install 目录位置。

### Python 命令找不到

在 Conda 环境中，使用：
- `python` 或 `python3`（取决于环境配置）
- 或使用完整路径：`$CONDA_PREFIX/bin/python`

## 更新日志

- **2026-01-17**: 创建 ros2_yolo 融合环境
- **2026-01-17**: 创建 robotics 项目配置
- **2026-01-17**: 创建项目创建工具
