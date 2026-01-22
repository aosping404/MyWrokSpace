#include <QApplication>
#include <rclcpp/rclcpp.hpp>
#include "my_visualization_tool/main_window.hpp"

int main(int argc, char *argv[])
{
    // 初始化 ROS2
    rclcpp::init(argc, argv);
    
    // 创建 Qt 应用程序
    QApplication app(argc, argv);
    
    app.setApplicationName("my_visualization_tool");
    app.setOrganizationName("MyCompany");
    
    // 创建主窗口
    my_visualization_tool::MainWindow window;
    window.show();
    
    // 运行 Qt 事件循环
    int result = app.exec();
    
    // 关闭 ROS2
    rclcpp::shutdown();
    
    return result;
}
