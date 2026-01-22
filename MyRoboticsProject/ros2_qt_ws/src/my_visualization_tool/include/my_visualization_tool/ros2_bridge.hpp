#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QTimer>
#include <memory>
#include "image_viewer.hpp"

namespace my_visualization_tool {

class ROS2Bridge : public QObject
{
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
};

} // namespace my_visualization_tool
