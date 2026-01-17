cmake_minimum_required(VERSION 3.16)
project(qt_test VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 或 Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# 设置自动处理 MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源文件
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
)

# 头文件
set(HEADERS
    include/MainWindow.h
)

# UI 文件
set(UI_FILES
    ui/MainWindow.ui
)

# 资源文件
set(RESOURCES
    resources/resources.qrc
)

# 创建可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCES}
)

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
