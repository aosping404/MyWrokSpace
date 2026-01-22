#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
ROS2 + OpenCV + YOLO26 + MoveIt2 项目创建工具
用于创建融合计算机视觉和机器人运动规划的 ROS2 包
"""

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
    """创建 ROS2 项目文件夹结构"""
    project_path = Path(project_name)
    
    # 检查项目是否已存在
    if project_path.exists():
        print_colored(f"警告: 项目文件夹 '{project_name}' 已存在", Colors.YELLOW)
        response = input("是否继续? (y/n): ")
        if response.lower() != 'y':
            print_colored("操作已取消", Colors.RED)
            sys.exit(1)
    
    # 创建 ROS2 包文件夹结构
    dirs = [
        project_path / "src" / project_name,
        project_path / "src" / project_name / "include" / project_name,
        project_path / "src" / project_name / "src",
        project_path / "src" / project_name / "launch",
        project_path / "src" / project_name / "config",
        project_path / "src" / project_name / "rviz",
        project_path / "src" / project_name / "models",  # YOLO 模型目录
        project_path / "build",
        project_path / "install",
        project_path / "log",
        project_path / ".vscode"
    ]
    
    for dir_path in dirs:
        dir_path.mkdir(parents=True, exist_ok=True)
        print_colored(f"✓ 创建文件夹: {dir_path}", Colors.GREEN)
    
    return project_path

def create_package_xml(project_path, project_name):
    """创建 ROS2 package.xml"""
    package_content = f'''<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="3">
  <name>{project_name}</name>
  <version>0.0.1</version>
  <description>ROS2 package integrating OpenCV, YOLO26, and MoveIt2 for robotics vision and manipulation</description>
  <maintainer email="your_email@example.com">Your Name</maintainer>
  <license>MIT</license>

  <buildtool_depend>ament_cmake</buildtool_depend>

  <!-- ROS2 依赖 -->
  <depend>rclcpp</depend>
  <depend>rclcpp_components</depend>
  <depend>sensor_msgs</depend>
  <depend>geometry_msgs</depend>
  <depend>std_msgs</depend>
  <depend>cv_bridge</depend>
  <depend>image_transport</depend>
  <depend>tf2</depend>
  <depend>tf2_ros</depend>
  <depend>tf2_geometry_msgs</depend>

  <!-- MoveIt2 依赖 -->
  <depend>moveit_core</depend>
  <depend>moveit_ros_planning_interface</depend>
  <depend>moveit_ros_planning</depend>
  <depend>moveit_ros_move_group</depend>
  <depend>moveit_kinematics</depend>
  <depend>moveit_planners</depend>
  <depend>moveit_msgs</depend>

  <!-- 视觉依赖 -->
  <depend>vision_msgs</depend>

  <!-- OpenCV (通过 cv_bridge，但可能需要系统安装) -->
  <!-- YOLO26 (通过 Python binding 或直接集成) -->

  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
'''
    package_file = project_path / "src" / project_name / "package.xml"
    package_file.write_text(package_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {package_file}", Colors.GREEN)

def create_cmakelists(project_path, project_name):
    """创建 ROS2 CMakeLists.txt"""
    cmake_content = f'''cmake_minimum_required(VERSION 3.8)
project({project_name})

# 默认使用 C++17
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

# MoveIt2 依赖
find_package(moveit_core REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_ros_planning REQUIRED)
find_package(moveit_ros_move_group REQUIRED)
find_package(moveit_kinematics REQUIRED)

# OpenCV (系统安装)
find_package(OpenCV REQUIRED)

# 包含目录
include_directories(
  include/${{PROJECT_NAME}}
  ${{OpenCV_INCLUDE_DIRS}}
)

# 源文件
set(SOURCES
  src/{project_name}_node.cpp
  src/vision_processor.cpp
  src/moveit_controller.cpp
)

# 头文件
set(HEADERS
  include/${{PROJECT_NAME}}/vision_processor.hpp
  include/${{PROJECT_NAME}}/moveit_controller.hpp
)

# 创建可执行文件
add_executable(${{PROJECT_NAME}}_node ${{SOURCES}})
ament_target_dependencies(${{PROJECT_NAME}}_node
  rclcpp
  rclcpp_components
  sensor_msgs
  geometry_msgs
  std_msgs
  cv_bridge
  image_transport
  tf2
  tf2_ros
  tf2_geometry_msgs
  moveit_core
  moveit_ros_planning_interface
  moveit_ros_planning
  moveit_ros_move_group
)

# 链接 OpenCV
target_link_libraries(${{PROJECT_NAME}}_node
  ${{OpenCV_LIBS}}
)

# 安装
install(TARGETS
  ${{PROJECT_NAME}}_node
  DESTINATION lib/${{PROJECT_NAME}}
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  DIRECTORY launch config rviz models
  DESTINATION share/${{PROJECT_NAME}}
)

# 安装依赖
ament_export_include_directories(include)
ament_export_dependencies(
  rclcpp
  rclcpp_components
  sensor_msgs
  geometry_msgs
  std_msgs
  cv_bridge
  image_transport
  tf2
  tf2_ros
  tf2_geometry_msgs
  moveit_core
  moveit_ros_planning_interface
  moveit_ros_planning
  moveit_ros_move_group
)

ament_package()
'''
    cmake_file = project_path / "src" / project_name / "CMakeLists.txt"
    cmake_file.write_text(cmake_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {cmake_file}", Colors.GREEN)

def create_main_node(project_path, project_name):
    """创建主节点文件"""
    main_node_content = f'''#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include "{project_name}/vision_processor.hpp"
#include "{project_name}/moveit_controller.hpp"

using namespace std::chrono_literals;

class {project_name.capitalize()}Node : public rclcpp::Node
{{
public:
    {project_name.capitalize()}Node()
    : Node("{project_name}_node")
    {{
        RCLCPP_INFO(this->get_logger(), "{project_name} node started");
        
        // 组件将在 init() 方法中初始化（需要先创建 shared_ptr）
        
        // 创建图像订阅者
        image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/camera/image_raw", 10,
            std::bind(&{project_name.capitalize()}Node::imageCallback, this, std::placeholders::_1));
        
        // 创建定时器用于主循环
        timer_ = this->create_wall_timer(
            100ms, std::bind(&{project_name.capitalize()}Node::timerCallback, this));
    }}
    
    // 初始化组件（在对象被 shared_ptr 管理后调用）
    void init()
    {{
        vision_processor_ = std::make_shared<VisionProcessor>(shared_from_this());
        moveit_controller_ = std::make_shared<MoveItController>(shared_from_this());
        RCLCPP_INFO(this->get_logger(), "Initialization complete");
    }}

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
    {{
        try {{
            // 转换 ROS 图像消息为 OpenCV Mat
            cv_bridge::CvImagePtr cv_ptr;
            
            // 尝试不同的编码格式
            std::string encoding = msg->encoding;
            
            if (encoding == sensor_msgs::image_encodings::BGR8) {{
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            }} else if (encoding == sensor_msgs::image_encodings::RGB8) {{
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
                // RGB8 需要转换为 BGR8（OpenCV 使用 BGR）
                cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
            }} else if (encoding == sensor_msgs::image_encodings::MONO8) {{
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
                // 灰度图转换为 BGR
                cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_GRAY2BGR);
            }} else if (encoding == "yuyv" || encoding == "yuv422_yuy2" || 
                       encoding == sensor_msgs::image_encodings::YUV422_YUY2) {{
                // YUYV/YUY2/YUV422_YUY2 格式（USB 摄像头常用）
                try {{
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
                }} catch (cv_bridge::Exception& e) {{
                    // 如果失败，尝试先转 RGB8 再转 BGR8
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                }}
            }} else if (encoding == "mjpeg" || encoding == "jpeg" || encoding == "jpg" || 
                       encoding == "mjpeg2rgb" || encoding == "rgb8") {{
                // Motion-JPEG 或 RGB8 格式
                if (encoding == "mjpeg2rgb" || encoding == "rgb8") {{
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                }} else {{
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
                }}
            }} else {{
                // 尝试自动转换到 BGR8
                try {{
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
                }} catch (cv_bridge::Exception& e) {{
                    // 如果失败，尝试使用原始编码
                    cv_ptr = cv_bridge::toCvCopy(msg);
                    if (cv_ptr->image.channels() == 1) {{
                        cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_GRAY2BGR);
                    }} else if (cv_ptr->image.channels() == 3) {{
                        cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                    }}
                }}
            }}
            
            // 使用 YOLO26 进行目标检测
            auto detections = vision_processor_->detectObjects(cv_ptr->image);
            
            // 处理检测结果并发送到 MoveIt2
            processDetections(detections);
            
        }} catch (cv_bridge::Exception& e) {{
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            RCLCPP_ERROR(this->get_logger(), "图像编码格式: %s", msg->encoding.c_str());
        }}
    }}
    
    void processDetections(const std::vector<Detection>& detections)
    {{
        for (const auto& detection : detections) {{
            RCLCPP_INFO(this->get_logger(), 
                "Detected: %s (confidence: %.2f) at [%.2f, %.2f]",
                detection.class_name.c_str(), detection.confidence,
                detection.center_x, detection.center_y);
            
            // 根据检测结果规划机器人运动
            // moveit_controller_->planToTarget(detection);
        }}
    }}
    
    void timerCallback()
    {{
        // 主循环逻辑
    }}
    
    std::shared_ptr<VisionProcessor> vision_processor_;
    std::shared_ptr<MoveItController> moveit_controller_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
    rclcpp::TimerBase::SharedPtr timer_;
}};

int main(int argc, char * argv[])
{{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<{project_name.capitalize()}Node>();
    node->init();  // 初始化组件（此时对象已被 shared_ptr 管理）
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}}
'''
    main_file = project_path / "src" / project_name / "src" / f"{project_name}_node.cpp"
    main_file.write_text(main_node_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {main_file}", Colors.GREEN)

def create_vision_processor_header(project_path, project_name):
    """创建视觉处理器头文件"""
    header_content = f'''#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <rclcpp/rclcpp.hpp>

struct Detection {{
    std::string class_name;
    float confidence;
    float center_x;
    float center_y;
    float width;
    float height;
    cv::Rect bbox;
}};

class VisionProcessor
{{
public:
    explicit VisionProcessor(rclcpp::Node::SharedPtr node);
    ~VisionProcessor();
    
    // YOLO26 目标检测
    std::vector<Detection> detectObjects(const cv::Mat& image);
    
    // OpenCV 图像处理
    cv::Mat preprocessImage(const cv::Mat& image);
    void drawDetections(cv::Mat& image, const std::vector<Detection>& detections);
    
private:
    rclcpp::Node::SharedPtr node_;
    
    // YOLO26 模型路径
    std::string yolo_model_path_;
    
    // 初始化 YOLO 模型（如果需要 C++ 绑定）
    bool loadYOLOModel(const std::string& model_path);
}};
'''
    header_file = project_path / "src" / project_name / "include" / project_name / "vision_processor.hpp"
    header_file.write_text(header_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {header_file}", Colors.GREEN)

def create_vision_processor_impl(project_path, project_name):
    """创建视觉处理器实现文件"""
    impl_content = f'''#include "{project_name}/vision_processor.hpp"
#include <rclcpp/rclcpp.hpp>

VisionProcessor::VisionProcessor(rclcpp::Node::SharedPtr node)
: node_(node)
{{
    // 从参数服务器获取 YOLO 模型路径
    node_->declare_parameter("yolo_model_path", 
        "${{{{CMAKE_INSTALL_PREFIX}}}}/share/{project_name}/models/yolo26/yolo26n.pt");
    node_->get_parameter("yolo_model_path", yolo_model_path_);
    
    RCLCPP_INFO(node_->get_logger(), "VisionProcessor initialized");
    RCLCPP_INFO(node_->get_logger(), "YOLO model path: %s", yolo_model_path_.c_str());
    
    // 注意：YOLO26 主要通过 Python 绑定使用
    // C++ 集成可以使用：
    // 1. Python C API 调用 Python YOLO 脚本
    // 2. ONNX Runtime 加载 YOLO ONNX 模型
    // 3. TensorRT 部署（NVIDIA GPU）
}}

VisionProcessor::~VisionProcessor()
{{
}}

cv::Mat VisionProcessor::preprocessImage(const cv::Mat& image)
{{
    cv::Mat processed;
    cv::resize(image, processed, cv::Size(640, 480));
    return processed;
}}

std::vector<Detection> VisionProcessor::detectObjects(const cv::Mat& image)
{{
    std::vector<Detection> detections;
    
    // 预处理图像
    cv::Mat processed = preprocessImage(image);
    
    // TODO: 集成 YOLO26 检测
    // 方式1: 调用 Python 脚本（使用 Python C API 或 subprocess）
    // 方式2: 使用 ONNX Runtime 加载 YOLO ONNX 模型
    // 方式3: 使用 TensorRT 加载 YOLO 模型（NVIDIA GPU）
    
    // 示例检测（占位符）
    Detection dummy_detection;
    dummy_detection.class_name = "object";
    dummy_detection.confidence = 0.95f;
    dummy_detection.center_x = image.cols / 2.0f;
    dummy_detection.center_y = image.rows / 2.0f;
    dummy_detection.width = 100.0f;
    dummy_detection.height = 100.0f;
    detections.push_back(dummy_detection);
    
    return detections;
}}

void VisionProcessor::drawDetections(cv::Mat& image, const std::vector<Detection>& detections)
{{
    for (const auto& det : detections) {{
        // 绘制边界框
        cv::rectangle(image, det.bbox, cv::Scalar(0, 255, 0), 2);
        
        // 绘制标签和置信度
        std::string label = det.class_name + ": " + std::to_string(det.confidence);
        cv::putText(image, label, cv::Point(det.bbox.x, det.bbox.y - 10),
            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }}
}}

bool VisionProcessor::loadYOLOModel(const std::string& model_path)
{{
    // TODO: 加载 YOLO 模型
    // 返回是否成功加载
    return true;
}}
'''
    impl_file = project_path / "src" / project_name / "src" / "vision_processor.cpp"
    impl_file.write_text(impl_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {impl_file}", Colors.GREEN)

def create_moveit_controller_header(project_path, project_name):
    """创建 MoveIt2 控制器头文件"""
    header_content = '''#pragma once

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <vector>
#include <string>
#include <rclcpp/rclcpp.hpp>

class MoveItController
{
public:
    explicit MoveItController(rclcpp::Node::SharedPtr node);
    ~MoveItController();
    
    // 运动规划
    bool planToTarget(const geometry_msgs::msg::Pose& target_pose);
    bool planToNamedTarget(const std::string& target_name);
    
    // 执行运动
    bool executePlan();
    
    // 获取当前状态
    geometry_msgs::msg::Pose getCurrentPose();
    
    // 碰撞检测
    bool checkCollision(const geometry_msgs::msg::Pose& pose);
    
private:
    rclcpp::Node::SharedPtr node_;
    
    // MoveIt2 接口
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_;
    
    // 规划组名称
    std::string planning_group_;
    
    // 当前规划结果
    moveit::planning_interface::MoveGroupInterface::Plan current_plan_;
};
'''
    header_file = project_path / "src" / project_name / "include" / project_name / "moveit_controller.hpp"
    header_file.write_text(header_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {header_file}", Colors.GREEN)

def create_moveit_controller_impl(project_path, project_name):
    """创建 MoveIt2 控制器实现文件"""
    impl_content = f'''#include "{project_name}/moveit_controller.hpp"
#include <rclcpp/rclcpp.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

MoveItController::MoveItController(rclcpp::Node::SharedPtr node)
: node_(node)
{{
    // 从参数服务器获取规划组名称
    node_->declare_parameter("planning_group", "manipulator");
    node_->get_parameter("planning_group", planning_group_);
    
    // 初始化 MoveGroup 接口
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
        node_, planning_group_);
    
    // 初始化规划场景接口
    planning_scene_ = std::make_shared<moveit::planning_interface::PlanningSceneInterface>();
    
    RCLCPP_INFO(node_->get_logger(), "MoveItController initialized");
    RCLCPP_INFO(node_->get_logger(), "Planning group: %s", planning_group_.c_str());
    RCLCPP_INFO(node_->get_logger(), "Reference frame: %s", 
        move_group_->getPlanningFrame().c_str());
}}

MoveItController::~MoveItController()
{{
}}

bool MoveItController::planToTarget(const geometry_msgs::msg::Pose& target_pose)
{{
    // 设置目标位姿
    move_group_->setPoseTarget(target_pose);
    
    // 规划运动
    moveit::core::MoveItErrorCode success = move_group_->plan(current_plan_);
    
    if (success == moveit::core::MoveItErrorCode::SUCCESS)
    {{
        RCLCPP_INFO(node_->get_logger(), "Motion planning succeeded");
        return true;
    }}
    else
    {{
        RCLCPP_ERROR(node_->get_logger(), "Motion planning failed");
        return false;
    }}
}}

bool MoveItController::planToNamedTarget(const std::string& target_name)
{{
    move_group_->setNamedTarget(target_name);
    
    moveit::core::MoveItErrorCode success = move_group_->plan(current_plan_);
    
    if (success == moveit::core::MoveItErrorCode::SUCCESS)
    {{
        RCLCPP_INFO(node_->get_logger(), "Motion planning to '%s' succeeded", target_name.c_str());
        return true;
    }}
    else
    {{
        RCLCPP_ERROR(node_->get_logger(), "Motion planning to '%s' failed", target_name.c_str());
        return false;
    }}
}}

bool MoveItController::executePlan()
{{
    moveit::core::MoveItErrorCode success = move_group_->execute(current_plan_);
    
    if (success == moveit::core::MoveItErrorCode::SUCCESS)
    {{
        RCLCPP_INFO(node_->get_logger(), "Motion execution succeeded");
        return true;
    }}
    else
    {{
        RCLCPP_ERROR(node_->get_logger(), "Motion execution failed");
        return false;
    }}
}}

geometry_msgs::msg::Pose MoveItController::getCurrentPose()
{{
    geometry_msgs::msg::PoseStamped current_pose_stamped = 
        move_group_->getCurrentPose();
    return current_pose_stamped.pose;
}}

bool MoveItController::checkCollision(const geometry_msgs::msg::Pose& /*pose*/)
{{
    // TODO: 实现碰撞检测
    // 可以使用 planning_scene_ 来检查碰撞
    return false;
}}
'''
    impl_file = project_path / "src" / project_name / "src" / "moveit_controller.cpp"
    impl_file.write_text(impl_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {impl_file}", Colors.GREEN)

def create_launch_file(project_path, project_name):
    """创建 ROS2 launch 文件"""
    launch_content = f'''from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, LogInfo
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
import os

def generate_launch_description():
    # 获取包路径
    pkg_share = FindPackageShare(package='{project_name}').find('{project_name}')
    
    # Launch 参数
    yolo_model_path_arg = DeclareLaunchArgument(
        'yolo_model_path',
        default_value=os.path.join(pkg_share, 'models', 'yolo26', 'yolo26n.pt'),
        description='Path to YOLO26 model file'
    )
    
    planning_group_arg = DeclareLaunchArgument(
        'planning_group',
        default_value='manipulator',
        description='MoveIt2 planning group name'
    )
    
    # 主节点
    {project_name}_node = Node(
        package='{project_name}',
        executable='{project_name}_node',
        name='{project_name}_node',
        output='screen',
        parameters=[{{
            'yolo_model_path': LaunchConfiguration('yolo_model_path'),
            'planning_group': LaunchConfiguration('planning_group'),
        }}]
    )
    
    return LaunchDescription([
        yolo_model_path_arg,
        planning_group_arg,
        {project_name}_node,
        LogInfo(msg=['Starting {project_name} node with YOLO26 and MoveIt2 integration']),
    ])
'''
    launch_file = project_path / "src" / project_name / "launch" / f"{project_name}.launch.py"
    launch_file.write_text(launch_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {launch_file}", Colors.GREEN)

def create_config_file(project_path, project_name):
    """创建配置文件"""
    config_content = '''# ROS2 + OpenCV + YOLO26 + MoveIt2 Configuration

# YOLO26 模型配置
yolo:
  model_path: "$(find {})/models/yolo26/yolo26n.pt"
  model_size: "nano"  # nano, small, medium, large, xlarge
  confidence_threshold: 0.5
  iou_threshold: 0.45
  input_size: [640, 640]

# MoveIt2 配置
moveit:
  planning_group: "manipulator"
  planning_time: 10.0
  num_planning_attempts: 10
  max_velocity_scaling_factor: 0.1
  max_acceleration_scaling_factor: 0.1

# 相机配置
camera:
  topic: "/camera/image_raw"
  frame_id: "camera_frame"
  width: 640
  height: 480

# 视觉-运动映射配置
vision_to_motion:
  enable_coordinate_transform: true
  target_frame: "base_link"
  camera_frame: "camera_frame"
'''.format(project_name)
    config_file = project_path / "src" / project_name / "config" / "config.yaml"
    config_file.write_text(config_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {config_file}", Colors.GREEN)

def create_vscode_tasks(project_path, project_name):
    """创建 VSCode tasks.json"""
    tasks_content = f'''{{
  "version": "2.0.0",
  "tasks": [
    {{
      "label": "colcon: Build",
      "type": "shell",
      "command": "colcon build",
      "args": [
        "--packages-select", "{project_name}",
        "--cmake-args", "-DCMAKE_BUILD_TYPE=Debug"
      ],
      "problemMatcher": ["$gcc"],
      "group": {{
        "kind": "build",
        "isDefault": true
      }},
      "options": {{
        "cwd": "${{workspaceFolder}}"
      }}
    }},
    {{
      "label": "colcon: Build All",
      "type": "shell",
      "command": "colcon build",
      "args": ["--cmake-args", "-DCMAKE_BUILD_TYPE=Debug"],
      "problemMatcher": ["$gcc"],
      "group": "build"
    }},
    {{
      "label": "colcon: Clean",
      "type": "shell",
      "command": "colcon build",
      "args": ["--cmake-clean-cache", "--packages-select", "{project_name}"],
      "problemMatcher": []
    }},
    {{
      "label": "ros2: Launch",
      "type": "shell",
      "command": "ros2 launch {project_name} {project_name}.launch.py",
      "problemMatcher": [],
      "options": {{
        "cwd": "${{workspaceFolder}}/install/{project_name}/share/{project_name}"
      }}
    }}
  ]
}}
'''
    tasks_file = project_path / ".vscode" / "tasks.json"
    tasks_file.write_text(tasks_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {tasks_file}", Colors.GREEN)

def create_vscode_launch(project_path, project_name):
    """创建 VSCode launch.json"""
    launch_content = f'''{{
  "version": "0.2.0",
  "configurations": [
    {{
      "name": "C++: Debug ROS2 Node",
      "type": "cppdbg",
      "request": "launch",
      "program": "${{workspaceFolder}}/install/{project_name}/lib/{project_name}/{project_name}_node",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${{workspaceFolder}}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        {{
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }}
      ],
      "preLaunchTask": "colcon: Build"
    }}
  ]
}}
'''
    launch_file = project_path / ".vscode" / "launch.json"
    launch_file.write_text(launch_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {launch_file}", Colors.GREEN)

def create_vscode_cpp_properties(project_path, project_name):
    """创建 VSCode c_cpp_properties.json"""
    cpp_properties_content = '''{
  "configurations": [
    {
      "name": "Linux",
      "includePath": [
        "${workspaceFolder}/**",
        "${workspaceFolder}/src/*/include/**",
        "/opt/ros/jazzy/include/**",
        "/opt/ros/jazzy/include",
        "/usr/include/eigen3",
        "/usr/include/pcl-1.10",
        "/usr/local/include/opencv4",
        "/usr/include/opencv4"
      ],
      "defines": [],
      "compilerPath": "/usr/bin/g++",
      "cStandard": "c17",
      "cppStandard": "c++17",
      "intelliSenseMode": "linux-gcc-x64",
      "configurationProvider": "ms-vscode.cmake-tools",
      "compileCommands": "${workspaceFolder}/build/*/compile_commands.json"
    }
  ],
  "version": 4
}
'''
    cpp_properties_file = project_path / ".vscode" / "c_cpp_properties.json"
    cpp_properties_file.write_text(cpp_properties_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {cpp_properties_file}", Colors.GREEN)

def create_vscode_settings(project_path):
    """创建 VSCode settings.json"""
    settings_content = '''{
  "cmake.configureOnOpen": false,
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "files.associations": {
    "*.launch.py": "python",
    "*.yaml": "yaml",
    "*.yml": "yaml"
  },
  "C_Cpp.default.includePath": [
    "${workspaceFolder}/**",
    "/opt/ros/jazzy/include/**",
    "/usr/include/opencv4/**"
  ]
}
'''
    settings_file = project_path / ".vscode" / "settings.json"
    settings_file.write_text(settings_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {settings_file}", Colors.GREEN)

def create_gitignore(project_path):
    """创建 .gitignore"""
    gitignore_content = '''# ROS2 构建目录
build/
install/
log/
.ros/

# CMake
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile
*.cmake

# 编译产物
*.o
*.a
*.so
*.exe

# IDE
.vscode/settings.json
.idea/
*.swp
*.swo
*~

# OS
.DS_Store
Thumbs.db

# Python
__pycache__/
*.py[cod]
*.pyc

# 模型文件（大文件）
*.pt
*.onnx
*.pth
models/*.pt
models/*.onnx

# 数据集
datasets/
*.jpg
*.png
*.mp4
'''
    gitignore_file = project_path / ".gitignore"
    gitignore_file.write_text(gitignore_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {gitignore_file}", Colors.GREEN)

def create_readme(project_path, project_name):
    """创建 README.md"""
    readme_content = f'''# {project_name}

ROS2 包 - 融合 OpenCV、YOLO26 和 MoveIt2 的机器人视觉与运动规划项目

## 项目描述

本项目整合了以下技术栈：

- **ROS2 Jazzy**: 机器人操作系统框架
- **OpenCV**: 计算机视觉库
- **YOLO26**: 实时目标检测模型
- **MoveIt2**: 机器人运动规划框架

实现从相机图像获取、YOLO26 目标检测到 MoveIt2 运动规划的完整流程。

## 项目结构

```
{project_name}/
├── src/{project_name}/          # ROS2 包源码
│   ├── include/{project_name}/  # 头文件
│   │   ├── vision_processor.hpp
│   │   └── moveit_controller.hpp
│   ├── src/                     # 源文件
│   │   ├── {project_name}_node.cpp
│   │   ├── vision_processor.cpp
│   │   └── moveit_controller.cpp
│   ├── launch/                  # Launch 文件
│   │   └── {project_name}.launch.py
│   ├── config/                  # 配置文件
│   │   └── config.yaml
│   ├── rviz/                    # RViz 配置文件
│   ├── models/                  # YOLO 模型目录
│   │   └── yolo26/
│   ├── CMakeLists.txt
│   └── package.xml
├── build/                       # 构建目录
├── install/                     # 安装目录
├── log/                         # 日志目录
└── .vscode/                     # VSCode 配置
```

## 前置要求

### 系统要求

- Ubuntu 24.04 (ROS2 Jazzy)
- CUDA 12.8 (可选，用于 GPU 加速)

### 安装依赖

```bash
# ROS2 Jazzy（如果未安装）
# 参考: https://docs.ros.org/en/jazzy/Installation.html

# 系统依赖
sudo apt-get update
sudo apt-get install -y \\
    build-essential \\
    cmake \\
    git \\
    libopencv-dev \\
    python3-pip \\
    python3-colcon-common-extensions

# ROS2 依赖
sudo apt-get install -y \\
    ros-jazzy-rclcpp \\
    ros-jazzy-rclcpp-components \\
    ros-jazzy-sensor-msgs \\
    ros-jazzy-geometry-msgs \\
    ros-jazzy-cv-bridge \\
    ros-jazzy-image-transport \\
    ros-jazzy-tf2 \\
    ros-jazzy-tf2-ros \\
    ros-jazzy-tf2-geometry-msgs

# MoveIt2 依赖
sudo apt-get install -y \\
    ros-jazzy-moveit-core \\
    ros-jazzy-moveit-ros-planning-interface \\
    ros-jazzy-moveit-ros-planning \\
    ros-jazzy-moveit-ros-move-group \\
    ros-jazzy-moveit-kinematics

# Python 依赖（YOLO26）
# 注意: 如果使用 ros2_yolo 环境，这些包已安装，无需重复安装
# conda activate ros2_yolo
# pip install ultralytics opencv-python torch torchvision
```

## 环境配置

### 推荐方式: 使用项目环境脚本

项目已配置 `robotics` 环境，使用 `ros2_yolo` Conda 环境，自动激活 ROS2、MoveIt2 和 CUDA：

```bash
cd ~/MyWorkSpace
source project_env.sh robotics
```

**robotics 环境包含**:
- ✅ **ros2_yolo** Conda 环境 (Python 3.12.12)
- ✅ **PyTorch 2.9.1+cu128** (CUDA 12.8)
- ✅ **YOLO26 (ultralytics 8.4.5)**
- ✅ **OpenCV 4.12.0**
- ✅ **ROS2 Jazzy**
- ✅ **MoveIt2** (如果已编译)
- ✅ **CUDA 12.8**

### 手动激活（如果不用项目环境脚本）

### 1. 激活 ROS2 环境

```bash
source /opt/ros/jazzy/setup.bash
```

### 2. 激活 MoveIt2 工作空间（如果使用）

```bash
# 注意: MoveIt2 install 目录在非标准位置
source ~/ws_moveit2/src/install/setup.bash
```

### 3. 使用项目环境脚本（推荐）

```bash
# 使用项目环境切换脚本（自动激活 ros2_yolo + ROS2 + MoveIt2 + CUDA）
cd ~/MyWorkSpace
source project_env.sh robotics
```

**环境说明**:
- **ros2_yolo**: Conda 环境（Python 3.12，包含 PyTorch、YOLO26、OpenCV）
- **ROS2 Jazzy**: 机器人操作系统
- **MoveIt2**: 运动规划框架
- **CUDA 12.8**: GPU 加速支持

## 构建项目

### 使用 colcon

```bash
# 在项目根目录下
colcon build --packages-select {project_name}

# 或构建所有包
colcon build

# 激活构建环境
source install/setup.bash
```

### 使用 VSCode

1. 打开 VSCode
2. 按 `Ctrl+Shift+B` 构建项目
3. 或使用终端执行 colcon 命令

## 运行项目

### 使用 Launch 文件

```bash
# 激活环境
source install/setup.bash

# 运行 Launch 文件
ros2 launch {project_name} {project_name}.launch.py
```

### 直接运行节点

```bash
# 激活环境
source install/setup.bash

# 运行节点
ros2 run {project_name} {project_name}_node
```

## YOLO26 集成

### 模型放置

将 YOLO26 模型文件放置在：

```
src/{project_name}/models/yolo26/yolo26n.pt
```

### 集成方式

由于使用 **ros2_yolo** 环境（已包含 PyTorch、YOLO26），YOLO26 可以通过以下方式集成：

#### 方式 1: Python C API（推荐，适合 ros2_yolo 环境）

使用 Python C API 直接调用 YOLO Python 代码（ros2_yolo 环境已包含所有依赖）：

```cpp
// 在 vision_processor.cpp 中
#include <Python.h>

std::vector<Detection> VisionProcessor::detectObjects(const cv::Mat& image) {{
    Py_Initialize();
    // 确保使用 ros2_yolo 环境的 Python
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os; sys.path.insert(0, os.path.join(os.environ.get('CONDA_PREFIX', ''), 'lib', 'python3.12', 'site-packages'))");
    
    // 调用 YOLO Python 脚本
    // ...
    
    Py_Finalize();
}}
```

#### 方式 2: ROS2 服务（最简单）

创建独立的 Python YOLO 服务节点，C++ 节点通过 ROS2 服务调用：

```python
# yolo_service_node.py (在 ros2_yolo 环境中运行)
import rclpy
from rclpy.node import Node
from ultralytics import YOLO
# ...
```

#### 方式 3: ONNX Runtime（纯 C++）

将 YOLO 模型转换为 ONNX 格式，使用 C++ ONNX Runtime：

```bash
# 在 ros2_yolo 环境中转换模型
conda activate ros2_yolo
python -c "from ultralytics import YOLO; model = YOLO('yolo26n.pt'); model.export(format='onnx')"
```

#### 方式 4: TensorRT（NVIDIA GPU 优化）

将 YOLO 模型转换为 TensorRT 格式以获得最佳性能。

当前实现提供了占位符，建议使用**方式 1（Python C API）**或**方式 2（ROS2 服务）**，因为 ros2_yolo 环境已包含所有必要的 Python 依赖。

### 使用示例

```cpp
// 在 vision_processor.cpp 中
std::vector<Detection> detections = detectObjects(image);
```

## MoveIt2 集成

### 配置规划组

在 Launch 文件中指定规划组：

```bash
ros2 launch {project_name} {project_name}.launch.py planning_group:=manipulator
```

### 使用示例

```cpp
// 在 moveit_controller.cpp 中
geometry_msgs::msg::Pose target_pose;
// 设置目标位姿
target_pose.position.x = 0.5;
target_pose.position.y = 0.0;
target_pose.position.z = 0.3;
moveit_controller_->planToTarget(target_pose);
moveit_controller_->executePlan();
```

## 开发指南

### 添加新功能

1. **扩展视觉处理**：在 `vision_processor.hpp/cpp` 中添加新的图像处理函数
2. **扩展运动规划**：在 `moveit_controller.hpp/cpp` 中添加新的运动规划函数
3. **添加新节点**：创建新的 C++ 源文件并在 `CMakeLists.txt` 中注册

### 调试技巧

1. **使用 RViz 可视化**：
   ```bash
   rviz2
   ```

2. **查看话题**：
   ```bash
   ros2 topic list
   ros2 topic echo /camera/image_raw
   ```

3. **查看节点信息**：
   ```bash
   ros2 node list
   ros2 node info /{project_name}_node
   ```

## VSCode 开发

### 配置说明

项目已配置完整的 VSCode 开发环境：

- **tasks.json**: colcon 构建任务
- **launch.json**: C++ 调试配置
- **c_cpp_properties.json**: IntelliSense 配置（包含 ROS2、OpenCV 路径）
- **settings.json**: 工作区设置

### 使用步骤

1. 打开项目文件夹：`code {project_name}/`
2. 构建项目：`Ctrl+Shift+B`
3. 调试节点：`F5`（需要先构建）
4. 运行 Launch：使用终端执行 `ros2 launch` 命令

## 故障排除

### 问题1: MoveIt2 找不到规划组

**解决方法**：
- 确保机器人描述（URDF/SRDF）已正确加载
- 检查规划组名称是否正确
- 验证 MoveIt2 工作空间已正确 source

### 问题2: OpenCV 找不到

**解决方法**：
```bash
sudo apt-get install libopencv-dev
# 或使用 conda 环境的 OpenCV
```

### 问题3: YOLO26 模型无法加载

**解决方法**：
- 检查模型文件路径是否正确
- 确保模型文件格式支持（.pt, .onnx 等）
- 验证 Python 环境和 ultralytics 包已安装

### 问题4: 编译错误

**解决方法**：
```bash
# 清理构建目录
colcon build --cmake-clean-cache

# 重新构建
colcon build --packages-select {project_name}
```

## 许可证

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！

## 参考链接

- [ROS2 Jazzy 文档](https://docs.ros.org/en/jazzy/)
- [MoveIt2 文档](https://moveit.ros.org/)
- [OpenCV 文档](https://docs.opencv.org/)
- [YOLO26 GitHub](https://github.com/ultralytics/ultralytics)
'''
    readme_file = project_path / "README.md"
    readme_file.write_text(readme_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {readme_file}", Colors.GREEN)

def main():
    print_colored("=" * 50, Colors.BLUE)
    print_colored("  ROS2 + OpenCV + YOLO26 + MoveIt2 项目创建工具", Colors.BLUE)
    print_colored("=" * 50, Colors.BLUE)
    print()
    
    # 获取项目名称
    if len(sys.argv) > 1:
        project_name = sys.argv[1]
    else:
        project_name = input("请输入项目名称: ").strip()
    
    if not project_name:
        print_colored("错误: 项目名称不能为空", Colors.RED)
        sys.exit(1)
    
    # 验证项目名称（ROS2 包名规范：小写字母、数字、下划线）
    if not project_name.replace('_', '').isalnum() or not project_name[0].isalpha():
        print_colored("警告: 项目名称不符合 ROS2 包命名规范", Colors.YELLOW)
        print_colored("建议: 使用小写字母、数字和下划线，且以字母开头", Colors.YELLOW)
        response = input("是否继续? (y/n): ")
        if response.lower() != 'y':
            sys.exit(1)
    
    print()
    print_colored("正在创建项目结构...", Colors.BLUE)
    print()
    
    # 创建项目结构
    project_path = create_project_structure(project_name)
    
    # 创建 ROS2 包文件
    create_package_xml(project_path, project_name)
    create_cmakelists(project_path, project_name)
    
    # 创建源代码文件
    create_main_node(project_path, project_name)
    create_vision_processor_header(project_path, project_name)
    create_vision_processor_impl(project_path, project_name)
    create_moveit_controller_header(project_path, project_name)
    create_moveit_controller_impl(project_path, project_name)
    
    # 创建配置和启动文件
    create_launch_file(project_path, project_name)
    create_config_file(project_path, project_name)
    
    # 创建 VSCode 配置文件
    create_vscode_tasks(project_path, project_name)
    create_vscode_launch(project_path, project_name)
    create_vscode_cpp_properties(project_path, project_name)
    create_vscode_settings(project_path)
    
    # 创建其他文件
    create_gitignore(project_path)
    create_readme(project_path, project_name)
    
    print()
    print_colored(f"✓ 项目 '{project_name}' 创建成功!", Colors.GREEN)
    print()
    print_colored("项目结构:", Colors.BLUE)
    
    # 显示项目结构（简化版）
    print(f"  {project_name}/")
    print(f"  ├── src/{project_name}/")
    print(f"  │   ├── include/{project_name}/")
    print(f"  │   ├── src/")
    print(f"  │   ├── launch/")
    print(f"  │   ├── config/")
    print(f"  │   ├── models/")
    print(f"  │   ├── CMakeLists.txt")
    print(f"  │   └── package.xml")
    print(f"  ├── build/")
    print(f"  ├── install/")
    print(f"  ├── log/")
    print(f"  └── .vscode/")
    print()
    
    print_colored("下一步:", Colors.YELLOW)
    print(f"  1. cd {project_name}")
    print("  2. source ~/MyWorkSpace/project_env.sh robotics  # 激活 ros2_yolo + ROS2 + MoveIt2 + CUDA")
    print("  3. colcon build --packages-select {}".format(project_name))
    print("  4. source install/setup.bash")
    print("  5. code .  # 在 VSCode 中打开")
    print()
    print_colored("环境配置提示:", Colors.YELLOW)
    print("  - 使用 robotics 环境配置（ros2_yolo Conda 环境）")
    print("  - 命令: source ~/MyWorkSpace/project_env.sh robotics")
    print("  - YOLO26 模型需要放置在: src/{}/models/yolo26/".format(project_name))
    print("  - 环境已包含: PyTorch 2.9.1+cu128, YOLO26, OpenCV 4.12.0, ROS2 Python")
    print()

if __name__ == "__main__":
    main()