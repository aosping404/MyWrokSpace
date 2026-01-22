#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
ROS2 + Qt 可视化工具项目创建脚本
用于创建融合 ROS2、OpenCV、YOLO26 和 MoveIt2 的可视化工具
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
        project_path / "include" / project_name,
        project_path / "ui",
        project_path / "resources",
        project_path / "build",
        project_path / ".vscode"
    ]
    
    for dir_path in dirs:
        dir_path.mkdir(parents=True, exist_ok=True)
        print_colored(f"✓ 创建文件夹: {dir_path}", Colors.GREEN)
    
    return project_path

def create_cmakelists(project_path, project_name):
    """创建 CMakeLists.txt"""
    cmake_content = f'''cmake_minimum_required(VERSION 3.16)
project({project_name} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 或 Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${{QT_VERSION_MAJOR}} REQUIRED COMPONENTS Core Widgets)

# ROS2 依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)

# OpenCV
find_package(OpenCV REQUIRED)

# 设置自动处理 MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置 AUTOUIC 搜索路径
set(CMAKE_AUTOUIC_SEARCH_PATHS ${{CMAKE_SOURCE_DIR}}/ui)

# 包含目录
include_directories(
    ${{CMAKE_SOURCE_DIR}}/include
    ${{CMAKE_BINARY_DIR}}
    ${{OpenCV_INCLUDE_DIRS}}
)

# 源文件
set(SOURCES
    src/main.cpp
    src/main_window.cpp
    src/image_viewer.cpp
    src/ros2_bridge.cpp
)

# 头文件
set(HEADERS
    include/{project_name}/main_window.hpp
    include/{project_name}/image_viewer.hpp
    include/{project_name}/ros2_bridge.hpp
)

# UI 文件
set(UI_FILES
    ui/MainWindow.ui
)

# 创建可执行文件
if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(${{PROJECT_NAME}}
        ${{SOURCES}}
        ${{HEADERS}}
        ${{UI_FILES}}
    )
else()
    add_executable(${{PROJECT_NAME}}
        ${{SOURCES}}
        ${{HEADERS}}
        ${{UI_FILES}}
    )
endif()

# 链接库
target_link_libraries(${{PROJECT_NAME}} PRIVATE
    Qt${{QT_VERSION_MAJOR}}::Core
    Qt${{QT_VERSION_MAJOR}}::Widgets
    rclcpp::rclcpp
    cv_bridge::cv_bridge
    ${{OpenCV_LIBS}}
)

# 注意：sensor_msgs 和 geometry_msgs 只需要包含头文件，不需要链接库
# ROS2消息类型通过头文件包含即可使用

# 添加 ROS2 消息类型包含目录
target_include_directories(${{PROJECT_NAME}} PRIVATE
    ${{CMAKE_BINARY_DIR}}
    ${{CMAKE_CURRENT_BINARY_DIR}}
    ${{CMAKE_BINARY_DIR}}/${{PROJECT_NAME}}_autogen/include
)

# 确保目标包含构建目录，以便找到自动生成的 ui_*.h 文件
# ROS2消息类型的头文件会自动通过find_package找到，无需手动添加包含目录

# 安装
install(TARGETS ${{PROJECT_NAME}}
    DESTINATION lib/${{PROJECT_NAME}}
)

# 安装 UI 文件（可选）
install(DIRECTORY ui
    DESTINATION share/${{PROJECT_NAME}}
    OPTIONAL
)

ament_package()
'''
    cmake_file = project_path / "CMakeLists.txt"
    cmake_file.write_text(cmake_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {cmake_file}", Colors.GREEN)

def create_package_xml(project_path, project_name):
    """创建 ROS2 package.xml"""
    package_content = f'''<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="3">
  <name>{project_name}</name>
  <version>1.0.0</version>
  <description>Qt visualization tool for ROS2 robotics with YOLO26 and MoveIt2</description>
  <maintainer email="your_email@example.com">Your Name</maintainer>
  <license>MIT</license>

  <buildtool_depend>ament_cmake</buildtool_depend>

  <depend>rclcpp</depend>
  <depend>sensor_msgs</depend>
  <depend>geometry_msgs</depend>
  <depend>cv_bridge</depend>
  <depend>image_transport</depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
'''
    package_file = project_path / "package.xml"
    package_file.write_text(package_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {package_file}", Colors.GREEN)

def create_main_cpp(project_path, project_name):
    """创建 main.cpp"""
    main_cpp_content = f'''#include <QApplication>
#include <rclcpp/rclcpp.hpp>
#include "{project_name}/main_window.hpp"

int main(int argc, char *argv[])
{{
    // 初始化 ROS2
    rclcpp::init(argc, argv);
    
    // 创建 Qt 应用程序
    QApplication app(argc, argv);
    
    app.setApplicationName("{project_name}");
    app.setOrganizationName("MyCompany");
    
    // 创建主窗口
    {project_name.replace("-", "_")}::MainWindow window;
    window.show();
    
    // 运行 Qt 事件循环
    int result = app.exec();
    
    // 关闭 ROS2
    rclcpp::shutdown();
    
    return result;
}}
'''
    main_file = project_path / "src" / "main.cpp"
    main_file.write_text(main_cpp_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {main_file}", Colors.GREEN)

def create_main_window_header(project_path, project_name):
    """创建 MainWindow 头文件"""
    header_content = f'''#pragma once

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "{project_name}/image_viewer.hpp"
#include "{project_name}/ros2_bridge.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {{ class MainWindow; }}
QT_END_NAMESPACE

namespace {project_name.replace("-", "_")} {{

class MainWindow : public QMainWindow
{{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onImageReceived(const cv::Mat& image);
    void onDetectionReceived(const std::vector<Detection>& detections);
    void updateRos2Status();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<rclcpp::Node> ros_node_;
    std::unique_ptr<ROS2Bridge> ros_bridge_;
    std::unique_ptr<ImageViewer> image_viewer_;
    QTimer *status_timer_;
}};

}} // namespace {project_name.replace("-", "_")}
'''
    header_file = project_path / "include" / project_name / "main_window.hpp"
    header_file.write_text(header_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {header_file}", Colors.GREEN)

def create_image_viewer_header(project_path, project_name):
    """创建 ImageViewer 头文件"""
    header_content = f'''#pragma once

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

struct Detection {{
    std::string class_name;
    float confidence;
    float center_x;
    float center_y;
    float width;
    float height;
    cv::Rect bbox;
}};

namespace {project_name.replace("-", "_")} {{

class ImageViewer : public QWidget
{{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    void setImage(const cv::Mat& image);
    void drawDetections(const std::vector<Detection>& detections);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateScaledPixmap();
    
    QLabel *image_label_;
    cv::Mat current_image_;
    QPixmap original_pixmap_;  // 保存原始图像，用于缩放
    
    QPixmap matToQPixmap(const cv::Mat& mat);
}};

}} // namespace {project_name.replace("-", "_")}
'''
    header_file = project_path / "include" / project_name / "image_viewer.hpp"
    header_file.write_text(header_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {header_file}", Colors.GREEN)

def create_ros2_bridge_header(project_path, project_name):
    """创建 ROS2Bridge 头文件"""
    header_content = f'''#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QTimer>
#include <memory>
#include "image_viewer.hpp"

namespace {project_name.replace("-", "_")} {{

class ROS2Bridge : public QObject
{{
    Q_OBJECT

public:
    explicit ROS2Bridge(rclcpp::Node::SharedPtr node, QObject *parent = nullptr);
    ~ROS2Bridge();

signals:
    void imageReceived(const cv::Mat& image);
    void detectionReceived(const std::vector<Detection>& detections);

private slots:
    void spinRos2();

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg);
    
    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
    QTimer *spin_timer_;  // 使用 Qt 定时器而不是 ROS2 定时器
    int message_count_;   // 消息计数器
}};

}} // namespace {project_name.replace("-", "_")}
'''
    header_file = project_path / "include" / project_name / "ros2_bridge.hpp"
    header_file.write_text(header_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {header_file}", Colors.GREEN)

def create_main_window_ui(project_path):
    """创建 MainWindow.ui"""
    ui_content = '''<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>1920</width>
    <height>1080</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>ROS2 Robotics Visualization Tool</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <widget class="QLabel" name="status_label">
      <property name="text">
       <string>ROS2 Status: Connecting...</string>
      </property>
     </widget>
    </item>
    <item>
     <widget class="QLabel" name="image_viewer">
      <property name="text">
       <string>No Image</string>
      </property>
      <property name="alignment">
       <set>Qt::AlignCenter</set>
      </property>
      <property name="scaledContents">
       <bool>true</bool>
      </property>
     </widget>
    </item>
   </layout>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>
'''
    ui_file = project_path / "ui" / "MainWindow.ui"
    ui_file.write_text(ui_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {ui_file}", Colors.GREEN)

def create_main_window_impl(project_path, project_name):
    """创建 MainWindow 实现文件"""
    impl_content = f'''#include "{project_name}/main_window.hpp"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>

namespace {project_name.replace("-", "_")} {{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{{
    ui->setupUi(this);
    
    // 创建 ROS2 节点
    ros_node_ = std::make_shared<rclcpp::Node>("{project_name}_node");
    
    // 创建 ROS2 桥接
    ros_bridge_ = std::make_unique<ROS2Bridge>(ros_node_, this);
    
    // 创建图像查看器
    image_viewer_ = std::make_unique<ImageViewer>(this);
    
    // 将图像查看器添加到布局（替换原有的 image_viewer QLabel）
    // 注意：UI 文件中的 image_viewer 是 QLabel，我们将其替换为自定义的 ImageViewer
    if (ui->image_viewer) {{
        // 获取 image_viewer 在布局中的位置
        int index = ui->verticalLayout->indexOf(ui->image_viewer);
        ui->verticalLayout->removeWidget(ui->image_viewer);
        delete ui->image_viewer;
        ui->image_viewer = nullptr;
        // 在相同位置插入 ImageViewer
        ui->verticalLayout->insertWidget(index, image_viewer_.get());
    }} else {{
        // 如果没有 image_viewer，直接添加到布局末尾
        ui->verticalLayout->addWidget(image_viewer_.get());
    }}
    
    // 连接信号和槽
    connect(ros_bridge_.get(), &ROS2Bridge::imageReceived,
            this, &MainWindow::onImageReceived);
    connect(ros_bridge_.get(), &ROS2Bridge::detectionReceived,
            this, &MainWindow::onDetectionReceived);
    
    // 创建状态更新定时器
    status_timer_ = new QTimer(this);
    connect(status_timer_, &QTimer::timeout, this, &MainWindow::updateRos2Status);
    status_timer_->start(1000);  // 每秒更新一次
    
    // 初始化状态
    updateRos2Status();
}}

MainWindow::~MainWindow()
{{
    delete ui;
}}

void MainWindow::onImageReceived(const cv::Mat& image)
{{
    if (image_viewer_) {{
        image_viewer_->setImage(image);
    }}
}}

void MainWindow::onDetectionReceived(const std::vector<Detection>& detections)
{{
    if (image_viewer_) {{
        image_viewer_->drawDetections(detections);
    }}
    
    // 更新状态标签
    QString status = QString("检测到 %1 个目标").arg(detections.size());
    ui->status_label->setText(status);
}}

void MainWindow::updateRos2Status()
{{
    if (ros_node_) {{
        QString status = QString("ROS2 Status: 已连接 (节点: %1)")
            .arg(ros_node_->get_name());
        ui->status_label->setText(status);
    }} else {{
        ui->status_label->setText("ROS2 Status: 未连接");
    }}
}}

}} // namespace {project_name.replace("-", "_")}
'''
    impl_file = project_path / "src" / "main_window.cpp"
    impl_file.write_text(impl_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {impl_file}", Colors.GREEN)

def create_image_viewer_impl(project_path, project_name):
    """创建 ImageViewer 实现文件"""
    impl_content = f'''#include "{project_name}/image_viewer.hpp"
#include <QVBoxLayout>
#include <QDebug>
#include <QImage>
#include <QResizeEvent>

namespace {project_name.replace("-", "_")} {{

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
    , image_label_(new QLabel(this))
{{
    image_label_->setAlignment(Qt::AlignCenter);
    image_label_->setText("等待图像...");
    image_label_->setScaledContents(false);  // 不使用自动缩放，手动控制以保持宽高比
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(image_label_);
    layout->setContentsMargins(0, 0, 0, 0);
    
    setLayout(layout);
    
    // 启用大小策略，让图像查看器可以扩展
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    image_label_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}}

void ImageViewer::setImage(const cv::Mat& image)
{{
    if (image.empty()) {{
        qWarning() << "ImageViewer: 图像为空";
        image_label_->setText("图像为空");
        return;
    }}
    
    current_image_ = image.clone();
    
    // 转换 OpenCV Mat 为 QPixmap
    QPixmap pixmap = matToQPixmap(image);
    
    if (pixmap.isNull()) {{
        qWarning() << "ImageViewer: QPixmap 转换失败";
        image_label_->setText("图像转换失败");
        return;
    }}
    
    // 保存原始 pixmap（用于缩放）
    original_pixmap_ = pixmap;
    
    // 更新显示（会自动缩放以适应窗口）
    updateScaledPixmap();
}}

void ImageViewer::updateScaledPixmap()
{{
    if (original_pixmap_.isNull()) {{
        return;
    }}
    
    // 获取可用空间（减去边距）
    QSize available_size = image_label_->size();
    
    if (available_size.width() <= 0 || available_size.height() <= 0) {{
        // 如果还没有大小，使用原始尺寸
        image_label_->setPixmap(original_pixmap_);
        return;
    }}
    
    // 计算缩放比例，保持宽高比
    QSize pixmap_size = original_pixmap_.size();
    QSize scaled_size = pixmap_size.scaled(available_size, Qt::KeepAspectRatio);
    
    // 缩放 pixmap（使用平滑变换）
    QPixmap scaled_pixmap = original_pixmap_.scaled(scaled_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    image_label_->setPixmap(scaled_pixmap);
}}

void ImageViewer::resizeEvent(QResizeEvent *event)
{{
    QWidget::resizeEvent(event);
    // 窗口大小改变时，更新缩放后的图像
    updateScaledPixmap();
}}

void ImageViewer::drawDetections(const std::vector<Detection>& detections)
{{
    if (current_image_.empty()) {{
        return;
    }}
    
    cv::Mat image_with_detections = current_image_.clone();
    
    for (const auto& det : detections) {{
        // 绘制边界框
        cv::rectangle(image_with_detections, det.bbox,
                     cv::Scalar(0, 255, 0), 2);
        
        // 绘制标签和置信度
        std::string label = det.class_name + ": " + 
                           std::to_string(static_cast<int>(det.confidence * 100)) + "%";
        
        cv::Point text_pos(det.bbox.x, det.bbox.y - 10);
        if (text_pos.y < 0) {{
            text_pos.y = det.bbox.y + 20;
        }}
        
        cv::putText(image_with_detections, label, text_pos,
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }}
    
    // 更新显示的图像
    QPixmap pixmap = matToQPixmap(image_with_detections);
    original_pixmap_ = pixmap;
    updateScaledPixmap();
}}

QPixmap ImageViewer::matToQPixmap(const cv::Mat& mat)
{{
    if (mat.empty()) {{
        qWarning() << "ImageViewer: 输入图像为空";
        return QPixmap();
    }}
    
    cv::Mat rgb_mat;
    
    // 先确保数据是连续的
    cv::Mat mat_cont = mat.isContinuous() ? mat : mat.clone();
    
    if (mat_cont.channels() == 1) {{
        cv::cvtColor(mat_cont, rgb_mat, cv::COLOR_GRAY2RGB);
    }} else if (mat_cont.channels() == 3) {{
        // 从 BGR 转换为 RGB（OpenCV 使用 BGR，Qt 使用 RGB）
        cv::cvtColor(mat_cont, rgb_mat, cv::COLOR_BGR2RGB);
    }} else if (mat_cont.channels() == 4) {{
        cv::cvtColor(mat_cont, rgb_mat, cv::COLOR_BGRA2RGB);
    }} else {{
        qWarning() << "ImageViewer: 不支持的通道数:" << mat_cont.channels();
        return QPixmap();
    }}
    
    // 确保 RGB 数据是连续的
    if (!rgb_mat.isContinuous()) {{
        rgb_mat = rgb_mat.clone();
    }}
    
    // 创建 QImage（使用深拷贝，确保数据独立）
    QImage qimg(static_cast<const uchar*>(rgb_mat.data), 
                rgb_mat.cols, 
                rgb_mat.rows, 
                static_cast<int>(rgb_mat.step), 
                QImage::Format_RGB888);
    
    // 深拷贝图像数据（重要：确保 QPixmap 拥有独立的数据副本）
    QImage qimg_copy = qimg.copy();
    
    if (qimg_copy.isNull()) {{
        qWarning() << "ImageViewer: QImage 复制失败";
        return QPixmap();
    }}
    
    QPixmap pixmap = QPixmap::fromImage(qimg_copy);
    
    if (pixmap.isNull()) {{
        qWarning() << "ImageViewer: QPixmap 创建失败";
        return QPixmap();
    }}
    
    return pixmap;
}}

}} // namespace {project_name.replace("-", "_")}
'''
    impl_file = project_path / "src" / "image_viewer.cpp"
    impl_file.write_text(impl_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {impl_file}", Colors.GREEN)

def create_ros2_bridge_impl(project_path, project_name):
    """创建 ROS2Bridge 实现文件"""
    impl_content = f'''#include "{project_name}/ros2_bridge.hpp"
#include <QDebug>

namespace {project_name.replace("-", "_")} {{

ROS2Bridge::ROS2Bridge(rclcpp::Node::SharedPtr node, QObject *parent)
    : QObject(parent)
    , node_(node)
    , message_count_(0)
{{
    if (!node_) {{
        qWarning() << "ROS2Bridge: 节点指针为空";
        return;
    }}
    
    // 创建图像订阅者
    image_sub_ = node_->create_subscription<sensor_msgs::msg::Image>(
        "/camera/image_raw", 10,
        std::bind(&ROS2Bridge::imageCallback, this, std::placeholders::_1));
    
    // 使用 Qt 定时器处理 ROS2 消息（更可靠）
    spin_timer_ = new QTimer(this);
    connect(spin_timer_, &QTimer::timeout, this, &ROS2Bridge::spinRos2);
    spin_timer_->start(10);  // 每 10ms 处理一次消息
    
    qDebug() << "ROS2Bridge: 已订阅话题 /camera/image_raw";
}}

ROS2Bridge::~ROS2Bridge()
{{
    if (spin_timer_) {{
        spin_timer_->stop();
    }}
}}

void ROS2Bridge::spinRos2()
{{
    // 处理 ROS2 消息（非阻塞）
    rclcpp::spin_some(node_);
}}

void ROS2Bridge::imageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{{
    message_count_++;
    
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
                qWarning() << "ROS2Bridge: YUV422_YUY2 转 BGR8 失败:" << e.what();
                // 如果失败，尝试先转 RGB8 再转 BGR8
                try {{
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                }} catch (cv_bridge::Exception& e2) {{
                    qWarning() << "ROS2Bridge: 所有 YUV422 转换方法都失败:" << e2.what();
                    throw;
                }}
            }}
        }} else if (encoding == "mjpeg" || encoding == "jpeg" || encoding == "jpg" || 
                   encoding == "mjpeg2rgb" || encoding == "rgb8") {{
            // Motion-JPEG 或 RGB8 格式
            try {{
                // 如果是 mjpeg2rgb，已经是 RGB8 格式，需要转换为 BGR8
                if (encoding == "mjpeg2rgb" || encoding == "rgb8") {{
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                }} else {{
                    // MJPEG 可以直接转换为 BGR8
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
                }}
            }} catch (cv_bridge::Exception& e) {{
                qWarning() << "ROS2Bridge: MJPEG/RGB 转 BGR8 失败:" << e.what();
                // 尝试使用原始编码
                cv_ptr = cv_bridge::toCvCopy(msg);
                // 如果是单通道，转换为 BGR
                if (cv_ptr->image.channels() == 1) {{
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_GRAY2BGR);
                }} else if (cv_ptr->image.channels() == 3) {{
                    // 假设是 RGB，转换为 BGR
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                }}
            }}
        }} else {{
            // 尝试自动转换到 BGR8
            try {{
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            }} catch (cv_bridge::Exception& e) {{
                qWarning() << "ROS2Bridge: 自动转换失败:" << e.what();
                // 如果失败，尝试使用原始编码
                try {{
                    cv_ptr = cv_bridge::toCvCopy(msg);
                    // 如果是单通道，转换为 BGR
                    if (cv_ptr->image.channels() == 1) {{
                        cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_GRAY2BGR);
                    }} else if (cv_ptr->image.channels() == 3) {{
                        // 尝试 RGB 转 BGR
                        cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                    }}
                }} catch (cv_bridge::Exception& e2) {{
                    qWarning() << "ROS2Bridge: 所有转换方法都失败:" << e2.what();
                    throw;  // 重新抛出异常
                }}
            }}
        }}
        
        // 发出信号传递图像
        emit imageReceived(cv_ptr->image);
        
    }} catch (cv_bridge::Exception& e) {{
        qWarning() << "ROS2Bridge: cv_bridge 异常:" << e.what();
        qWarning() << "ROS2Bridge: 图像编码格式:" << msg->encoding.c_str();
        qWarning() << "ROS2Bridge: 尝试使用原始编码格式转换";
        
        // 尝试使用原始编码格式
        try {{
            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg);
            qDebug() << "ROS2Bridge: 使用原始编码转换成功";
            emit imageReceived(cv_ptr->image);
        }} catch (cv_bridge::Exception& e2) {{
            qWarning() << "ROS2Bridge: 原始编码转换也失败:" << e2.what();
        }}
    }}
}}

}} // namespace {project_name.replace("-", "_")}
'''
    impl_file = project_path / "src" / "ros2_bridge.cpp"
    impl_file.write_text(impl_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {impl_file}", Colors.GREEN)

def create_resources_qrc(project_path):
    """创建 resources.qrc"""
    qrc_content = '''<!DOCTYPE RCC>
<RCC version="1.0">
    <qresource>
        <!-- 可以在这里添加资源文件 -->
        <!-- <file>images/icon.png</file> -->
    </qresource>
</RCC>
'''
    qrc_file = project_path / "resources" / "resources.qrc"
    qrc_file.write_text(qrc_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {qrc_file}", Colors.GREEN)

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

# Qt
*.pro.user
*.pro.user.*
*.qmlc
*.jsc
*.autosave

# 自动生成的文件
ui_*.h
moc_*.cpp
qrc_*.cpp
*_autogen/
'''
    gitignore_file = project_path / ".gitignore"
    gitignore_file.write_text(gitignore_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {gitignore_file}", Colors.GREEN)

def create_readme(project_path, project_name):
    """创建 README.md"""
    readme_content = f'''# {project_name}

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
{project_name}/
├── CMakeLists.txt          # CMake 构建配置
├── package.xml             # ROS2 包描述文件
├── README.md               # 项目说明
├── .gitignore              # Git 忽略文件
├── include/                # 头文件目录
│   └── {project_name}/
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
sudo apt-get install -y \\
    build-essential \\
    cmake \\
    git \\
    libopencv-dev \\
    qt6-base-dev \\
    qt6-base-dev-tools

# ROS2 依赖
sudo apt-get install -y \\
    ros-jazzy-rclcpp \\
    ros-jazzy-sensor-msgs \\
    ros-jazzy-geometry-msgs \\
    ros-jazzy-cv-bridge \\
    ros-jazzy-image-transport \\
    python3-colcon-common-extensions
```

## 构建项目

### 使用 ROS2 工作空间（推荐）

```bash
# 1. 创建或进入 ROS2 工作空间
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src

# 2. 将项目复制到工作空间
cp -r /path/to/{project_name} .

# 3. 构建项目
cd ~/ros2_ws
colcon build --packages-select {project_name}

# 4. 设置环境
source install/setup.bash

# 5. 运行应用
ros2 run {project_name} {project_name}
```

### 直接使用 CMake 构建

```bash
cd {project_name}
mkdir -p build
cd build
cmake ..
cmake --build .
./{project_name}
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
ros2 run {project_name} {project_name}

# 或直接运行可执行文件
./build/{project_name}
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
'''
    readme_file = project_path / "README.md"
    readme_file.write_text(readme_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {readme_file}", Colors.GREEN)

def create_vscode_tasks(project_path, project_name):
    """创建 VSCode tasks.json"""
    tasks_content = f'''{{
  "version": "2.0.0",
  "tasks": [
    {{
      "label": "CMake: Configure",
      "type": "shell",
      "command": "cmake",
      "args": [
        "-B", "build",
        "-S", ".",
        "-DCMAKE_BUILD_TYPE=Debug"
      ],
      "problemMatcher": [],
      "group": "build"
    }},
    {{
      "label": "CMake: Build",
      "type": "shell",
      "command": "cmake",
      "args": [
        "--build", "build"
      ],
      "problemMatcher": ["$gcc"],
      "group": {{
        "kind": "build",
        "isDefault": true
      }}
    }},
    {{
      "label": "colcon: Build",
      "type": "shell",
      "command": "colcon build",
      "args": [
        "--packages-select", "{project_name}",
        "--cmake-args", "-DCMAKE_BUILD_TYPE=Debug"
      ],
      "problemMatcher": ["$gcc"],
      "group": "build",
      "options": {{
        "cwd": "${{workspaceFolder}}/.."
      }}
    }},
    {{
      "label": "ROS2: Run",
      "type": "shell",
      "command": "ros2 run {project_name} {project_name}",
      "problemMatcher": []
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
      "name": "C++: Debug {project_name}",
      "type": "cppdbg",
      "request": "launch",
      "program": "${{workspaceFolder}}/build/{project_name}",
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
      "preLaunchTask": "CMake: Build"
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
        "${workspaceFolder}/include/**",
        "/opt/ros/jazzy/include/**",
        "/usr/include/opencv4/**",
        "/usr/include/qt6/**",
        "/usr/include/x86_64-linux-gnu/qt6/**"
      ],
      "defines": [],
      "compilerPath": "/usr/bin/g++",
      "cStandard": "c17",
      "cppStandard": "c++17",
      "intelliSenseMode": "linux-gcc-x64",
      "configurationProvider": "ms-vscode.cmake-tools",
      "compileCommands": "${workspaceFolder}/build/compile_commands.json"
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
    "*.ui": "xml",
    "*.qrc": "xml"
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

def main():
    """主函数"""
    print_colored("=" * 60, Colors.BLUE)
    print_colored("  ROS2 + Qt 可视化工具项目创建脚本", Colors.BLUE)
    print_colored("=" * 60, Colors.BLUE)
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
    create_main_cpp(project_path, project_name)
    create_main_window_header(project_path, project_name)
    create_main_window_impl(project_path, project_name)
    create_image_viewer_header(project_path, project_name)
    create_image_viewer_impl(project_path, project_name)
    create_ros2_bridge_header(project_path, project_name)
    create_ros2_bridge_impl(project_path, project_name)
    
    # 创建 UI 和资源文件
    create_main_window_ui(project_path)
    create_resources_qrc(project_path)
    
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
    print(f"  {project_name}/")
    print(f"  ├── CMakeLists.txt")
    print(f"  ├── package.xml")
    print(f"  ├── README.md")
    print(f"  ├── .gitignore")
    print(f"  ├── include/{project_name}/")
    print(f"  │   ├── main_window.hpp")
    print(f"  │   ├── image_viewer.hpp")
    print(f"  │   └── ros2_bridge.hpp")
    print(f"  ├── src/")
    print(f"  │   ├── main.cpp")
    print(f"  │   ├── main_window.cpp")
    print(f"  │   ├── image_viewer.cpp")
    print(f"  │   └── ros2_bridge.cpp")
    print(f"  ├── ui/")
    print(f"  │   └── MainWindow.ui")
    print(f"  ├── resources/")
    print(f"  │   └── resources.qrc")
    print(f"  └── .vscode/")
    print()
    
    print_colored("下一步:", Colors.YELLOW)
    print(f"  1. cd {project_name}")
    print("  2. source /opt/ros/jazzy/setup.bash  # 激活 ROS2 环境")
    print("  3. mkdir -p build && cd build")
    print("  4. cmake ..")
    print("  5. cmake --build .")
    print(f"  6. ./{project_name}")
    print()
    print_colored("或使用 ROS2 工作空间:", Colors.YELLOW)
    print("  1. 将项目复制到 ~/ros2_ws/src/")
    print("  2. cd ~/ros2_ws")
    print("  3. colcon build --packages-select {}".format(project_name))
    print("  4. source install/setup.bash")
    print("  5. ros2 run {} {}".format(project_name, project_name))
    print()

if __name__ == "__main__":
    main()
