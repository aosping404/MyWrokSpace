# Robotics 项目环境配置说明

## 环境需求

Robotics 项目需要同时使用：
- **ROS2 Jazzy**: 机器人操作系统（使用系统 Python 3.12）
- **MoveIt2**: 机器人运动规划（依赖 ROS2）
- **OpenCV**: 计算机视觉库（系统安装）
- **YOLO26**: 深度学习目标检测（需要 PyTorch + CUDA）

## 已创建的配置文件

项目环境配置文件：`.project_env_configs/robotics.conf`

```bash
CONDA_ENV="base"
ROS_DISTRO="jazzy"
MOVEIT2_WS="$HOME/ws_moveit2"
CUDA_VERSION="12.8"
```

## 使用方法

### 1. 切换到 robotics 环境

```bash
cd ~/MyWorkSpace
source project_env.sh robotics
```

### 2. 验证环境

```bash
# 查看当前环境状态
source project_env.sh --show

# 或使用别名
penv-show
```

### 3. 检查各个组件

```bash
# 检查 ROS2
echo $ROS_DISTRO  # 应该显示: jazzy
ros2 pkg list | grep moveit

# 检查 MoveIt2
ls -la ~/ws_moveit2/install

# 检查 CUDA
echo $CUDA_HOME  # 应该显示: /usr/local/cuda-12.8
nvcc --version
```

## Python 环境说明

### 当前配置：使用系统 Python

`robotics.conf` 配置为使用 `base` Conda 环境（实际上接近系统 Python）。

**原因**:
- ROS2 Jazzy 使用系统 Python 3.12
- ROS2 的核心库在系统路径：`/opt/ros/jazzy/lib/python3.12/site-packages`
- MoveIt2 依赖 ROS2，也需要系统 Python

### YOLO26 集成选项

由于使用系统 Python，YOLO26 集成有以下选项：

#### 选项 A: Python C API（适合混合项目）

在 C++ 中通过 Python C API 调用 YOLO：

```cpp
// vision_processor.cpp
#include <Python.h>

std::vector<Detection> VisionProcessor::detectObjects(const cv::Mat& image) {
    Py_Initialize();
    // 调用 Python YOLO 脚本
    // ...
    Py_Finalize();
}
```

**需要**: 确保系统 Python 中可以导入 ultralytics：
```bash
pip3 install ultralytics opencv-python torch torchvision
```

#### 选项 B: ONNX Runtime（推荐，纯 C++）

1. 将 YOLO26 模型转换为 ONNX：
```python
from ultralytics import YOLO
model = YOLO('yolo26n.pt')
model.export(format='onnx')
```

2. 在 C++ 中使用 ONNX Runtime：
```cpp
#include <onnxruntime_cxx_api.h>
// 加载 ONNX 模型并推理
```

#### 选项 C: ROS2 服务（最简单）

创建独立的 Python YOLO 服务节点，C++ 节点通过 ROS2 服务调用。

## 如果创建融合的 Conda 环境

如果项目需要更多 Python 功能，可以创建 `ros2_yolo` 环境：

```bash
# 创建环境
conda create -n ros2_yolo python=3.12 -y
conda activate ros2_yolo

# 安装 YOLO26 依赖
conda install pytorch torchvision torchaudio pytorch-cuda=12.8 -c pytorch -c nvidia -y
pip install ultralytics opencv-python

# 修改 robotics.conf
CONDA_ENV="ros2_yolo"
```

**注意**: 使用 Conda 环境时，需要确保 ROS2 Python 路径正确：
```bash
export PYTHONPATH=/opt/ros/jazzy/lib/python3.12/site-packages:$PYTHONPATH
```

## 项目构建流程

```bash
# 1. 切换环境
cd ~/MyWorkSpace
source project_env.sh robotics

# 2. 进入项目目录
cd MyRoboticsProject/my_robotics_project

# 3. 构建项目
colcon build --packages-select my_robotics_project

# 4. 激活构建环境
source install/setup.bash

# 5. 运行项目
ros2 launch my_robotics_project my_robotics_project.launch.py
```

## VSCode 开发

1. 打开项目：
```bash
cd ~/MyWorkSpace/MyRoboticsProject/my_robotics_project
code .
```

2. 确保环境已激活（在终端中）：
```bash
source ~/MyWorkSpace/project_env.sh robotics
```

3. 使用 VSCode 任务：
- `Ctrl+Shift+B`: 构建项目（colcon build）
- `F5`: 调试 C++ 节点

## 依赖安装清单

确保以下依赖已安装：

```bash
# 系统依赖
sudo apt-get install -y \
    build-essential \
    cmake \
    libopencv-dev \
    python3-pip

# ROS2 依赖
sudo apt-get install -y \
    ros-jazzy-rclcpp \
    ros-jazzy-sensor-msgs \
    ros-jazzy-geometry-msgs \
    ros-jazzy-cv-bridge \
    ros-jazzy-image-transport \
    ros-jazzy-tf2-ros

# MoveIt2 依赖
sudo apt-get install -y \
    ros-jazzy-moveit-core \
    ros-jazzy-moveit-ros-planning-interface \
    ros-jazzy-moveit-ros-move-group

# Python 依赖（如果需要 Python YOLO）
pip3 install ultralytics opencv-python
```

## 常见问题

### Q1: ROS2 包找不到

**解决**: 确保 ROS2 环境已激活：
```bash
source /opt/ros/jazzy/setup.bash
source project_env.sh robotics
```

### Q2: MoveIt2 找不到

**解决**: 确保 MoveIt2 工作空间已编译：
```bash
cd ~/ws_moveit2
colcon build
source install/setup.bash
```

### Q3: CUDA 不可用

**解决**: 检查 CUDA 路径：
```bash
echo $CUDA_HOME
ls -la /usr/local/cuda-12.8
```

### Q4: Python 版本冲突

**解决**: 如果使用 Conda 环境，确保 Python 版本兼容：
```bash
python3 --version  # 应该是 3.12（ROS2 Jazzy 要求）
```

## 相关文档

- [项目环境脚本使用说明](../project_env_使用说明.md)
- [系统环境信息汇总](../系统环境信息汇总.md)
- [YOLO项目环境切换说明](../YOLO项目环境切换说明.md)
