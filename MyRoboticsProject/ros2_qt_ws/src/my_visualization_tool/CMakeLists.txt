cmake_minimum_required(VERSION 3.16)
project(my_visualization_tool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 或 Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

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
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}
    ${OpenCV_INCLUDE_DIRS}
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
    include/my_visualization_tool/main_window.hpp
    include/my_visualization_tool/image_viewer.hpp
    include/my_visualization_tool/ros2_bridge.hpp
)

# UI 文件
set(UI_FILES
    ui/MainWindow.ui
)

# 创建可执行文件
if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
    )
endif()

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    rclcpp::rclcpp
    cv_bridge::cv_bridge
    ${OpenCV_LIBS}
)

# 注意：sensor_msgs 和 geometry_msgs 只需要包含头文件，不需要链接库
# ROS2消息类型通过头文件包含即可使用

# 添加 ROS2 消息类型包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_autogen/include
)

# 确保目标包含构建目录，以便找到自动生成的 ui_*.h 文件
# ROS2消息类型的头文件会自动通过find_package找到，无需手动添加包含目录

# 安装
install(TARGETS ${PROJECT_NAME}
    DESTINATION lib/${PROJECT_NAME}
)

# 安装 UI 文件（可选）
install(DIRECTORY ui
    DESTINATION share/${PROJECT_NAME}
    OPTIONAL
)

ament_package()
