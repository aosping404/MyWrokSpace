#include "my_visualization_tool/ros2_bridge.hpp"
#include <QDebug>

namespace my_visualization_tool {

ROS2Bridge::ROS2Bridge(rclcpp::Node::SharedPtr node, QObject *parent)
    : QObject(parent)
    , node_(node)
    , message_count_(0)
{
    if (!node_) {
        qWarning() << "ROS2Bridge: 节点指针为空";
        return;
    }
    
    // 创建图像订阅者
    image_sub_ = node_->create_subscription<sensor_msgs::msg::Image>(
        "/camera/image_raw", 10,
        std::bind(&ROS2Bridge::imageCallback, this, std::placeholders::_1));
    
    // 使用 Qt 定时器处理 ROS2 消息（更可靠）
    spin_timer_ = new QTimer(this);
    connect(spin_timer_, &QTimer::timeout, this, &ROS2Bridge::spinRos2);
    spin_timer_->start(10);  // 每 10ms 处理一次消息
    
    qDebug() << "ROS2Bridge: 已订阅话题 /camera/image_raw";
    qDebug() << "ROS2Bridge: Qt 定时器已启动，每 10ms 处理 ROS2 消息";
}

ROS2Bridge::~ROS2Bridge()
{
    if (spin_timer_) {
        spin_timer_->stop();
    }
}

void ROS2Bridge::spinRos2()
{
    // 处理 ROS2 消息（非阻塞）
    rclcpp::spin_some(node_);
}

void ROS2Bridge::imageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    message_count_++;
    
    try {
        // 转换 ROS 图像消息为 OpenCV Mat
        cv_bridge::CvImagePtr cv_ptr;
        
        // 尝试不同的编码格式
        std::string encoding = msg->encoding;
        
        if (encoding == sensor_msgs::image_encodings::BGR8) {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } else if (encoding == sensor_msgs::image_encodings::RGB8) {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
            // RGB8 需要转换为 BGR8（OpenCV 使用 BGR）
            cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
        } else if (encoding == sensor_msgs::image_encodings::MONO8) {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
            // 灰度图转换为 BGR
            cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_GRAY2BGR);
        } else if (encoding == "yuyv" || encoding == "yuv422_yuy2" || 
                   encoding == sensor_msgs::image_encodings::YUV422_YUY2) {
            // YUYV/YUY2/YUV422_YUY2 格式（USB 摄像头常用）
            // 直接转换为 BGR8（cv_bridge 会自动处理 YUV422 到 BGR 的转换）
            try {
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            } catch (cv_bridge::Exception& e) {
                qWarning() << "ROS2Bridge: YUV422_YUY2 转 BGR8 失败:" << e.what();
                // 如果失败，尝试先转 RGB8 再转 BGR8
                try {
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                } catch (cv_bridge::Exception& e2) {
                    qWarning() << "ROS2Bridge: 所有 YUV422 转换方法都失败:" << e2.what();
                    throw;
                }
            }
        } else if (encoding == "mjpeg" || encoding == "jpeg" || encoding == "jpg" || 
                   encoding == "mjpeg2rgb" || encoding == "rgb8") {
            // Motion-JPEG 或 RGB8 格式
            try {
                // 如果是 mjpeg2rgb，已经是 RGB8 格式，需要转换为 BGR8
                if (encoding == "mjpeg2rgb" || encoding == "rgb8") {
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                } else {
                    // MJPEG 可以直接转换为 BGR8
                    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
                }
            } catch (cv_bridge::Exception& e) {
                qWarning() << "ROS2Bridge: MJPEG/RGB 转 BGR8 失败:" << e.what();
                // 尝试使用原始编码
                cv_ptr = cv_bridge::toCvCopy(msg);
                // 如果是单通道，转换为 BGR
                if (cv_ptr->image.channels() == 1) {
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_GRAY2BGR);
                } else if (cv_ptr->image.channels() == 3) {
                    // 假设是 RGB，转换为 BGR
                    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                }
            }
        } else {
            // 尝试自动转换到 BGR8
            try {
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            } catch (cv_bridge::Exception& e) {
                qWarning() << "ROS2Bridge: 自动转换失败:" << e.what();
                // 如果失败，尝试使用原始编码
                try {
                    cv_ptr = cv_bridge::toCvCopy(msg);
                    // 如果是单通道，转换为 BGR
                    if (cv_ptr->image.channels() == 1) {
                        cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_GRAY2BGR);
                    } else if (cv_ptr->image.channels() == 3) {
                        // 尝试 RGB 转 BGR
                        cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_RGB2BGR);
                    }
                } catch (cv_bridge::Exception& e2) {
                    qWarning() << "ROS2Bridge: 所有转换方法都失败:" << e2.what();
                    throw;  // 重新抛出异常
                }
            }
        }
        
        // 发出信号传递图像
        emit imageReceived(cv_ptr->image);
        
    } catch (cv_bridge::Exception& e) {
        qWarning() << "ROS2Bridge: cv_bridge 异常:" << e.what();
        qWarning() << "ROS2Bridge: 图像编码格式:" << msg->encoding.c_str();
        qWarning() << "ROS2Bridge: 尝试使用原始编码格式转换";
        
        // 尝试使用原始编码格式
        try {
            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg);
            qDebug() << "ROS2Bridge: 使用原始编码转换成功";
            emit imageReceived(cv_ptr->image);
        } catch (cv_bridge::Exception& e2) {
            qWarning() << "ROS2Bridge: 原始编码转换也失败:" << e2.what();
        }
    }
}

} // namespace my_visualization_tool
