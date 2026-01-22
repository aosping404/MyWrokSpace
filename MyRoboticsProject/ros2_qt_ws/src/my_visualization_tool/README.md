# my_visualization_tool

ROS2 + Qt 可视化工具项目

## 项目描述

本项目是一个融合 ROS2、Qt 和 OpenCV 的可视化工具，用于机器人视觉应用开发。

## 功能特性

- ✅ ROS2 图像订阅和显示
- ✅ OpenCV 图像处理
- ✅ Qt 图形界面
- ✅ 实时图像可视化
- ✅ 目标检测结果绘制（可扩展）

## 项目结构

```
my_visualization_tool/
├── CMakeLists.txt          # CMake 构建配置
├── package.xml             # ROS2 包描述文件
├── README.md               # 项目说明
├── .gitignore              # Git 忽略文件
├── include/                # 头文件目录
│   └── my_visualization_tool/
│       ├── main_window.hpp
│       ├── image_viewer.hpp
│       └── ros2_bridge.hpp
├── src/                    # 源文件目录
│   ├── main.cpp
│   ├── main_window.cpp
│   ├── image_viewer.cpp
│   └── ros2_bridge.cpp
├── ui/                     # Qt Designer UI 文件
│   └── MainWindow.ui
├── resources/              # Qt 资源文件
│   └── resources.qrc
└── build/                  # 构建目录
```

## 前置要求

### 系统要求

- Ubuntu 24.04 (ROS2 Jazzy)
- Qt 6.4.2 或更高版本
- CMake 3.16 或更高版本
- C++17 兼容的编译器

### 安装依赖

```bash
# ROS2 Jazzy（如果未安装）
# 参考: https://docs.ros.org/en/jazzy/Installation.html

# 系统依赖
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    libopencv-dev \
    qt6-base-dev \
    qt6-base-dev-tools

# ROS2 依赖
sudo apt-get install -y \
    ros-jazzy-rclcpp \
    ros-jazzy-sensor-msgs \
    ros-jazzy-geometry-msgs \
    ros-jazzy-cv-bridge \
    ros-jazzy-image-transport \
    python3-colcon-common-extensions
```

## 构建项目

### 使用 ROS2 工作空间（推荐）

```bash
# 1. 创建或进入 ROS2 工作空间
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src

# 2. 将项目复制到工作空间
cp -r /path/to/my_visualization_tool .

# 3. 构建项目
cd ~/ros2_ws
colcon build --packages-select my_visualization_tool

# 4. 设置环境
source install/setup.bash

# 5. 运行应用
ros2 run my_visualization_tool my_visualization_tool
```

### 直接使用 CMake 构建

```bash
cd my_visualization_tool
mkdir -p build
cd build
cmake ..
cmake --build .
./my_visualization_tool
```

## 运行项目

### 前提条件

确保有图像话题发布：

```bash
# 发布测试图像（需要先安装 usb_cam 或其他相机驱动）
ros2 run usb_cam usb_cam_node_exe
```

### 运行应用

```bash
# 使用 ROS2 运行
ros2 run my_visualization_tool my_visualization_tool

# 或直接运行可执行文件
./build/my_visualization_tool
```

## 配置

### 修改订阅话题

在 `src/ros2_bridge.cpp` 中修改：

```cpp
image_sub_ = node_->create_subscription<sensor_msgs::msg::Image>(
    "/your_topic_name", 10,  // 修改话题名称
    std::bind(&ROS2Bridge::imageCallback, this, std::placeholders::_1));
```

## 扩展功能

### 添加目标检测

1. 在 `image_viewer.hpp` 中定义 `Detection` 结构体（已包含）
2. 在 `ros2_bridge.cpp` 中添加检测逻辑
3. 通过信号 `detectionReceived` 传递检测结果
4. 在 `image_viewer.cpp` 中使用 `drawDetections` 绘制结果

### 集成 YOLO26

参考 `MyRoboticsProject/create_robotics_project.py` 中的 YOLO26 集成方式。

## 开发指南

### 添加新功能

1. **添加新的 Qt 组件**：
   - 在 `ui/MainWindow.ui` 中设计 UI
   - 在 `main_window.hpp/cpp` 中添加处理逻辑

2. **添加新的 ROS2 订阅**：
   - 在 `ros2_bridge.hpp/cpp` 中添加订阅者
   - 通过 Qt 信号传递数据到 UI 线程

3. **添加新的图像处理**：
   - 在 `image_viewer.hpp/cpp` 中添加处理函数
   - 使用 OpenCV 进行图像处理

## 故障排除

### 问题1: 找不到 OpenCV

```bash
sudo apt-get install libopencv-dev
```

### 问题2: 找不到 Qt

```bash
sudo apt-get install qt6-base-dev
```

### 问题3: ROS2 消息类型找不到

确保已 source ROS2 环境：
```bash
source /opt/ros/jazzy/setup.bash
```

### 问题4: 图像不显示

- 检查是否有图像话题发布：`ros2 topic list`
- 检查话题名称是否正确
- 检查图像编码格式

## 许可证

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！

## 参考链接

- [ROS2 Jazzy 文档](https://docs.ros.org/en/jazzy/)
- [Qt 官方文档](https://doc.qt.io/)
- [OpenCV 文档](https://docs.opencv.org/)
