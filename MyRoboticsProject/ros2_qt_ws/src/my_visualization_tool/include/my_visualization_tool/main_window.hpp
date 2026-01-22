#pragma once

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "my_visualization_tool/image_viewer.hpp"
#include "my_visualization_tool/ros2_bridge.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace my_visualization_tool {

class MainWindow : public QMainWindow
{
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
};

} // namespace my_visualization_tool
