# ROS2与Qt集成指南

## 概述

本指南说明如何在Qt项目中订阅ROS2话题，特别是订阅cup检测坐标。

## 方法一：使用C++ ROS2订阅者（推荐）

### 1. 创建ROS2订阅者类

创建以下文件：

#### `include/Ros2Subscriber.h`

```cpp
#pragma once

#include <QObject>
#include <QTimer>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>

class Ros2Subscriber : public QObject
{
    Q_OBJECT

public:
    explicit Ros2Subscriber(const QString& topic_name = "/cup_position", 
                           QObject* parent = nullptr);
    ~Ros2Subscriber();

    bool isConnected() const { return connected_; }

public slots:
    void start();
    void stop();

signals:
    // 当收到坐标消息时发出此信号
    void positionReceived(double x, double y, double z);
    // 连接状态改变时发出此信号
    void connectionStatusChanged(bool connected);

private slots:
    void spinRos2();  // 定期调用以处理ROS2消息

private:
    void cupPositionCallback(const geometry_msgs::msg::PointStamped::SharedPtr msg);

    std::shared_ptr<rclcpp::Node> node_;
    rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr subscription_;
    QTimer* spin_timer_;
    bool connected_;
    QString topic_name_;
};
```

#### `src/Ros2Subscriber.cpp`

```cpp
#include "Ros2Subscriber.h"
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>

Ros2Subscriber::Ros2Subscriber(const QString& topic_name, QObject* parent)
    : QObject(parent)
    , connected_(false)
    , topic_name_(topic_name)
{
    // 创建ROS2节点
    node_ = std::make_shared<rclcpp::Node>("qt_ros2_subscriber");
    
    // 创建定时器，定期处理ROS2消息
    spin_timer_ = new QTimer(this);
    connect(spin_timer_, &QTimer::timeout, this, &Ros2Subscriber::spinRos2);
    
    // 创建订阅者
    subscription_ = node_->create_subscription<geometry_msgs::msg::PointStamped>(
        topic_name_.toStdString(),
        10,
        std::bind(&Ros2Subscriber::cupPositionCallback, this, std::placeholders::_1)
    );
}

Ros2Subscriber::~Ros2Subscriber()
{
    stop();
}

void Ros2Subscriber::start()
{
    if (!spin_timer_->isActive()) {
        spin_timer_->start(10);  // 每10ms处理一次ROS2消息
        connected_ = true;
        emit connectionStatusChanged(true);
    }
}

void Ros2Subscriber::stop()
{
    if (spin_timer_->isActive()) {
        spin_timer_->stop();
        connected_ = false;
        emit connectionStatusChanged(false);
    }
}

void Ros2Subscriber::spinRos2()
{
    // 处理ROS2消息（非阻塞）
    rclcpp::spin_some(node_);
}

void Ros2Subscriber::cupPositionCallback(const geometry_msgs::msg::PointStamped::SharedPtr msg)
{
    // 发出Qt信号，传递坐标数据
    emit positionReceived(
        msg->point.x,
        msg->point.y,
        msg->point.z
    );
}
```

### 2. 在MainWindow中使用

#### 修改 `include/MainWindow.h`

```cpp
#pragma once

#include <QMainWindow>
#include "Ros2Subscriber.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();
    void onPositionReceived(double x, double y, double z);
    void onConnectionStatusChanged(bool connected);

private:
    Ui::MainWindow *ui;
    Ros2Subscriber* ros2_subscriber_;  // ROS2订阅者
};
```

#### 修改 `src/MainWindow.cpp`

```cpp
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 初始化ROS2订阅者
    ros2_subscriber_ = new Ros2Subscriber("/cup_position", this);
    
    // 连接信号和槽
    connect(ros2_subscriber_, &Ros2Subscriber::positionReceived,
            this, &MainWindow::onPositionReceived);
    connect(ros2_subscriber_, &Ros2Subscriber::connectionStatusChanged,
            this, &MainWindow::onConnectionStatusChanged);
    
    // 启动订阅者
    ros2_subscriber_->start();
    
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    
    setWindowTitle("Qt ROS2 订阅示例");
}

MainWindow::~MainWindow()
{
    if (ros2_subscriber_) {
        ros2_subscriber_->stop();
    }
    delete ui;
}

void MainWindow::onButtonClicked()
{
    QMessageBox::information(this, "提示", "按钮被点击了！");
}

void MainWindow::onPositionReceived(double x, double y, double z)
{
    // 更新UI显示坐标
    QString text = QString("Cup位置:\nX: %1\nY: %2\nZ: %3")
                   .arg(x, 0, 'f', 3)
                   .arg(y, 0, 'f', 3)
                   .arg(z, 0, 'f', 3);
    
    if (ui->label) {
        ui->label->setText(text);
    }
    
    // 或者输出到控制台
    qDebug() << "收到坐标:" << x << y << z;
}

void MainWindow::onConnectionStatusChanged(bool connected)
{
    if (connected) {
        qDebug() << "ROS2订阅者已连接";
    } else {
        qDebug() << "ROS2订阅者已断开";
    }
}
```

### 3. 修改CMakeLists.txt

在现有的CMakeLists.txt中添加ROS2依赖：

```cmake
# 在find_package部分添加
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

# 在源文件部分添加
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Ros2Subscriber.cpp  # 添加这行
)

# 在头文件部分添加
set(HEADERS
    include/MainWindow.h
    include/Ros2Subscriber.h  # 添加这行
)

# 在链接库部分添加
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    rclcpp::rclcpp  # 添加这行
    geometry_msgs::geometry_msgs  # 添加这行
)
```

### 4. 修改main.cpp

```cpp
#include <QApplication>
#include <rclcpp/rclcpp.hpp>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    // 初始化ROS2
    rclcpp::init(argc, argv);
    
    // 创建Qt应用程序
    QApplication app(argc, argv);
    
    app.setApplicationName("QtROS2App");
    
    // 创建主窗口
    MainWindow window;
    window.show();
    
    // 运行Qt事件循环
    int result = app.exec();
    
    // 关闭ROS2
    rclcpp::shutdown();
    
    return result;
}
```

## 方法二：使用Python脚本桥接（简单但性能较低）

如果不想修改C++代码，可以使用Python脚本作为桥接：

### `ros2_to_qt_bridge.py`

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PointStamped
import json
import sys

class Ros2ToQtBridge(Node):
    def __init__(self):
        super().__init__('ros2_to_qt_bridge')
        self.subscription = self.create_subscription(
            PointStamped,
            '/cup_position',
            self.position_callback,
            10
        )
    
    def position_callback(self, msg):
        # 输出JSON格式，Qt可以通过QProcess读取
        data = {
            'x': float(msg.point.x),
            'y': float(msg.point.y),
            'z': float(msg.point.z)
        }
        print(json.dumps(data))
        sys.stdout.flush()

def main():
    rclpy.init()
    node = Ros2ToQtBridge()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

然后在Qt中使用QProcess运行这个脚本。

## 编译和运行

### 1. 编译项目

```bash
cd /home/shigure/MyWorkSpace/MyQtProject/qttest  # 或你的项目目录
mkdir -p build && cd build
cmake ..
make
```

### 2. 运行前确保ROS2环境已设置

```bash
source /opt/ros/jazzy/setup.bash  # 或你的ROS2版本
```

### 3. 运行Qt应用

```bash
./bin/qttest
```

## 注意事项

1. **ROS2初始化**：必须在Qt应用启动前初始化ROS2，在退出前关闭
2. **线程安全**：ROS2回调在ROS2线程中执行，通过Qt信号传递到主线程是安全的
3. **消息频率**：spin_timer_的频率（10ms）可以根据需要调整
4. **话题名称**：确保话题名称与检测节点发布的一致（默认是`/cup_position`）

## 完整示例项目

我已经创建了完整的示例文件，你可以直接复制到你的项目中。
