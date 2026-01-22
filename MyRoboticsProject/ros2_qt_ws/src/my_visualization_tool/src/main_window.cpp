#include "my_visualization_tool/main_window.hpp"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>

namespace my_visualization_tool {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 创建 ROS2 节点
    ros_node_ = std::make_shared<rclcpp::Node>("my_visualization_tool_node");
    
    // 创建 ROS2 桥接
    ros_bridge_ = std::make_unique<ROS2Bridge>(ros_node_, this);
    
    // 创建图像查看器
    image_viewer_ = std::make_unique<ImageViewer>(this);
    
    // 将图像查看器添加到布局（替换原有的 image_viewer QLabel）
    // 注意：UI 文件中的 image_viewer 是 QLabel，我们将其替换为自定义的 ImageViewer
    if (ui->image_viewer) {
        // 获取 image_viewer 在布局中的位置
        int index = ui->verticalLayout->indexOf(ui->image_viewer);
        ui->verticalLayout->removeWidget(ui->image_viewer);
        delete ui->image_viewer;
        ui->image_viewer = nullptr;
        // 在相同位置插入 ImageViewer
        ui->verticalLayout->insertWidget(index, image_viewer_.get());
    } else {
        // 如果没有 image_viewer，直接添加到布局末尾
        ui->verticalLayout->addWidget(image_viewer_.get());
    }
    
    // 连接信号和槽
    bool connected1 = connect(ros_bridge_.get(), &ROS2Bridge::imageReceived,
        this, &MainWindow::onImageReceived);
    bool connected2 = connect(ros_bridge_.get(), &ROS2Bridge::detectionReceived,
        this, &MainWindow::onDetectionReceived);
    
    qDebug() << "MainWindow: 信号槽连接状态 - imageReceived:" << connected1
        << "detectionReceived:" << connected2;
    
    // 创建状态更新定时器
    status_timer_ = new QTimer(this);
    connect(status_timer_, &QTimer::timeout, this, &MainWindow::updateRos2Status);
    status_timer_->start(1000);  // 每秒更新一次
    
    // 初始化状态
    updateRos2Status();
    
    qDebug() << "my_visualization_tool MainWindow initialized";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onImageReceived(const cv::Mat& image)
{
    if (image_viewer_) {
        image_viewer_->setImage(image);
    } else {
        qWarning() << "MainWindow: image_viewer_ 为空！";
    }
}

void MainWindow::onDetectionReceived(const std::vector<Detection>& detections)
{
    if (image_viewer_) {
        image_viewer_->drawDetections(detections);
    }
    
    // 更新状态标签
    QString status = QString("检测到 %1 个目标").arg(detections.size());
    ui->status_label->setText(status);
}

void MainWindow::updateRos2Status()
{
    if (ros_node_) {
        QString status = QString("ROS2 Status: 已连接 (节点: %1)")
            .arg(ros_node_->get_name());
        ui->status_label->setText(status);
    } else {
        ui->status_label->setText("ROS2 Status: 未连接");
    }
}

} // namespace my_visualization_tool
