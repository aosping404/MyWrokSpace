cmake_minimum_required(VERSION 3.16)
project(QtFileTest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 或 Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# Qt 6 需要 LinguistTools 用于国际化
if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
endif()

# 设置自动处理 MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置 AUTOUIC 搜索路径，告诉 uic 在哪里查找 .ui 文件
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 确保构建目录被包含，以便找到自动生成的 ui_*.h 文件
# AUTOUIC 会自动处理，但显式添加可以确保 IntelliSense 正常工作
include_directories(${CMAKE_BINARY_DIR})

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

# 翻译文件
set(TS_FILES
    translations/QtFileTest_zh_CN.ts
)

# 创建可执行文件
# Qt 6 使用 qt_add_executable，Qt 5 使用 add_executable
if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${RESOURCES}
    )
    
    # Qt 6 国际化支持
    qt_add_translations(${PROJECT_NAME} TS_FILES ${TS_FILES})
    
    # 最终化可执行文件（Qt 6）
    # 注意：qt_add_translations 会自动触发最终化，如果出现重复调用警告，请注释掉下面这行
    # qt_finalize_executable(${PROJECT_NAME})
else()
    add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${RESOURCES}
    )
    
    # Qt 5 国际化支持
    qt5_add_translation(QM_FILES ${TS_FILES})
    add_custom_target(translations ALL DEPENDS ${QM_FILES})
endif()

# 链接 Qt 库（使用 PRIVATE 链接方式，更符合现代 CMake 实践）
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# 确保目标包含构建目录，以便找到自动生成的 ui_*.h 文件
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_autogen/include
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# iOS/macOS Bundle 配置
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
        MACOSX_BUNDLE_COPYRIGHT "Copyright © 2024"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.${PROJECT_NAME}"
    )
endif()

# Windows 可执行文件配置
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Android 支持（可选）
if(ANDROID)
    set(CMAKE_ANDROID_NDK ${ANDROID_NDK})
    set(CMAKE_SYSTEM_NAME Android)
endif()

# 安装配置
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 安装翻译文件
# 注意：翻译文件通常已包含在资源文件(.qrc)中，无需单独安装
# 如果需要单独安装翻译文件，可以使用以下方式：
# if(QT_VERSION_MAJOR EQUAL 6)
#     # Qt 6: 翻译文件已通过资源文件包含，或手动安装编译后的 .qm 文件
#     # file(GLOB QM_FILES "${CMAKE_BINARY_DIR}/translations/*.qm")
#     # install(FILES ${QM_FILES} DESTINATION translations OPTIONAL)
# else()
#     # Qt 5: 需要先编译翻译文件
#     if(QM_FILES)
#         install(FILES ${QM_FILES} DESTINATION translations)
#     endif()
# endif()
