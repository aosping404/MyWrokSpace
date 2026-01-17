#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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
        project_path / "include",
        project_path / "ui",
        project_path / "resources",
        project_path / "translations",
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

# Qt 6 需要 LinguistTools 用于国际化
if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
endif()

# 设置自动处理 MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置 AUTOUIC 搜索路径，告诉 uic 在哪里查找 .ui 文件
set(CMAKE_AUTOUIC_SEARCH_PATHS ${{CMAKE_SOURCE_DIR}}/ui)

# 包含目录
include_directories(${{CMAKE_SOURCE_DIR}}/include)

# 确保构建目录被包含，以便找到自动生成的 ui_*.h 文件
# AUTOUIC 会自动处理，但显式添加可以确保 IntelliSense 正常工作
include_directories(${{CMAKE_BINARY_DIR}})

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
    translations/{project_name}_zh_CN.ts
)

# 创建可执行文件
# Qt 6 使用 qt_add_executable，Qt 5 使用 add_executable
if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(${{PROJECT_NAME}}
        ${{SOURCES}}
        ${{HEADERS}}
        ${{UI_FILES}}
        ${{RESOURCES}}
    )
    
    # Qt 6 国际化支持
    qt_add_translations(${{PROJECT_NAME}} TS_FILES ${{TS_FILES}})
    
    # 最终化可执行文件（Qt 6）
    # 注意：qt_add_translations 会自动触发最终化，如果出现重复调用警告，请注释掉下面这行
    # qt_finalize_executable(${{PROJECT_NAME}})
else()
    add_executable(${{PROJECT_NAME}}
        ${{SOURCES}}
        ${{HEADERS}}
        ${{UI_FILES}}
        ${{RESOURCES}}
    )
    
    # Qt 5 国际化支持
    qt5_add_translation(QM_FILES ${{TS_FILES}})
    add_custom_target(translations ALL DEPENDS ${{QM_FILES}})
endif()

# 链接 Qt 库（使用 PRIVATE 链接方式，更符合现代 CMake 实践）
target_link_libraries(${{PROJECT_NAME}} PRIVATE
    Qt${{QT_VERSION_MAJOR}}::Core
    Qt${{QT_VERSION_MAJOR}}::Widgets
)

# 确保目标包含构建目录，以便找到自动生成的 ui_*.h 文件
target_include_directories(${{PROJECT_NAME}} PRIVATE
    ${{CMAKE_BINARY_DIR}}
    ${{CMAKE_CURRENT_BINARY_DIR}}
    ${{CMAKE_BINARY_DIR}}/${{PROJECT_NAME}}_autogen/include
)

# 设置输出目录
set_target_properties(${{PROJECT_NAME}} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_BINARY_DIR}}/bin
)

# iOS/macOS Bundle 配置
if(APPLE)
    set_target_properties(${{PROJECT_NAME}} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "${{PROJECT_NAME}}"
        MACOSX_BUNDLE_COPYRIGHT "Copyright © 2024"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.${{PROJECT_NAME}}"
    )
endif()

# Windows 可执行文件配置
if(WIN32)
    set_target_properties(${{PROJECT_NAME}} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Android 支持（可选）
if(ANDROID)
    set(CMAKE_ANDROID_NDK ${{ANDROID_NDK}})
    set(CMAKE_SYSTEM_NAME Android)
endif()

# 安装配置
install(TARGETS ${{PROJECT_NAME}}
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
#     # file(GLOB QM_FILES "${{CMAKE_BINARY_DIR}}/translations/*.qm")
#     # install(FILES ${{QM_FILES}} DESTINATION translations OPTIONAL)
# else()
#     # Qt 5: 需要先编译翻译文件
#     if(QM_FILES)
#         install(FILES ${{QM_FILES}} DESTINATION translations)
#     endif()
# endif()
'''
    cmake_file = project_path / "CMakeLists.txt"
    cmake_file.write_text(cmake_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {cmake_file}", Colors.GREEN)

def create_tasks_json(project_path):
    """创建 tasks.json"""
    tasks_content = '''{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "CMake: Configure",
      "type": "shell",
      "command": "cmake",
      "args": [
        "-B",
        "${workspaceFolder}/build",
        "-S",
        "${workspaceFolder}",
        "-DCMAKE_BUILD_TYPE=Debug"
      ],
      "problemMatcher": [],
      "group": "build"
    },
    {
      "label": "CMake: Build",
      "type": "shell",
      "command": "cmake",
      "args": [
        "--build",
        "${workspaceFolder}/build",
        "--config",
        "Debug"
      ],
      "problemMatcher": ["$gcc"],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    },
    {
      "label": "CMake: Clean",
      "type": "shell",
      "command": "cmake",
      "args": [
        "--build",
        "${workspaceFolder}/build",
        "--target",
        "clean"
      ],
      "problemMatcher": []
    }
  ]
}
'''
    tasks_file = project_path / ".vscode" / "tasks.json"
    tasks_file.write_text(tasks_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {tasks_file}", Colors.GREEN)

def create_launch_json(project_path, project_name):
    """创建 launch.json"""
    launch_content = f'''{{
  "version": "0.2.0",
  "configurations": [
    {{
      "name": "C/C++: Debug Qt Application",
      "type": "cppdbg",
      "request": "launch",
      "program": "${{workspaceFolder}}/build/bin/{project_name}",
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

def create_c_cpp_properties_json(project_path):
    """创建 c_cpp_properties.json"""
    cpp_properties_content = '''{
  "configurations": [
    {
      "name": "Linux",
      "includePath": [
        "${workspaceFolder}/**",
        "${workspaceFolder}/include",
        "${workspaceFolder}/build",
        "${workspaceFolder}/build/**",
        "${workspaceFolder}/build/${{workspaceFolderBasename}}_autogen/include",
        "/usr/include/x86_64-linux-gnu/qt6",
        "/usr/include/x86_64-linux-gnu/qt6/QtCore",
        "/usr/include/x86_64-linux-gnu/qt6/QtGui",
        "/usr/include/x86_64-linux-gnu/qt6/QtWidgets",
        "/usr/lib/x86_64-linux-gnu/qt6/mkspecs/linux-g++"
      ],
      "defines": [
        "QT_CORE_LIB",
        "QT_GUI_LIB",
        "QT_WIDGETS_LIB"
      ],
      "compilerPath": "/usr/bin/g++",
      "cStandard": "c11",
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

def create_settings_json(project_path):
    """创建 settings.json"""
    settings_content = '''{
  "cmake.configureOnOpen": true,
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "cmake.generator": "Unix Makefiles",
  "files.associations": {
    "*.ui": "xml",
    "*.qrc": "xml"
  }
}
'''
    settings_file = project_path / ".vscode" / "settings.json"
    settings_file.write_text(settings_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {settings_file}", Colors.GREEN)

def create_main_cpp(project_path, project_name):
    """创建示例 main.cpp"""
    main_cpp_content = f'''#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "MainWindow.h"

int main(int argc, char *argv[])
{{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("{project_name}");
    app.setOrganizationName("MyCompany");
    app.setOrganizationDomain("example.com");

    // 加载翻译文件
    // Qt 6 的 qt_add_translations 会自动处理翻译文件部署
    // 翻译文件会从应用程序目录或标准位置自动加载
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {{
        const QString baseName = "{project_name}_" + QLocale(locale).name();
        // 首先尝试从应用程序目录加载（Qt 6 自动部署的位置）
        if (translator.load(baseName, app.applicationDirPath())) {{
            app.installTranslator(&translator);
            break;
        }}
        // 如果未找到，尝试从资源文件加载（如果手动添加到资源文件）
        if (translator.load(":/i18n/" + baseName)) {{
            app.installTranslator(&translator);
            break;
        }}
    }}

    MainWindow window;
    window.show();

    return app.exec();
}}
'''
    main_cpp_file = project_path / "src" / "main.cpp"
    main_cpp_file.write_text(main_cpp_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {main_cpp_file}", Colors.GREEN)

def create_mainwindow_h(project_path):
    """创建 MainWindow.h"""
    mainwindow_h_content = '''#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // 必须与 .ui 文件中的类名一致
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();

private:
    Ui::MainWindow *ui; // UI 指针，用于访问 .ui 文件中定义的控件
};
'''
    mainwindow_h_file = project_path / "include" / "MainWindow.h"
    mainwindow_h_file.write_text(mainwindow_h_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {mainwindow_h_file}", Colors.GREEN)

def create_mainwindow_cpp(project_path):
    """创建 MainWindow.cpp"""
    mainwindow_cpp_content = '''#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    
    // 现在可以通过 ui-> 访问 .ui 文件中定义的控件
    // 根据 MainWindow.ui 文件，有一个名为 pushButton 的按钮和一个名为 label 的标签
    // 连接按钮的点击信号到槽函数
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    
    setWindowTitle("Qt Application");
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

void MainWindow::onButtonClicked()
{
    QMessageBox::information(this, "提示", "按钮被点击了！\\n这是一个 Qt 应用程序示例。");
}
'''
    mainwindow_cpp_file = project_path / "src" / "MainWindow.cpp"
    mainwindow_cpp_file.write_text(mainwindow_cpp_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {mainwindow_cpp_file}", Colors.GREEN)

def create_mainwindow_ui(project_path):
    """创建 MainWindow.ui"""
    ui_content = '''<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>400</width>
    <height>300</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Qt Application</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <widget class="QLabel" name="label">
      <property name="text">
       <string>欢迎使用 Qt 应用程序!</string>
      </property>
      <property name="alignment">
       <set>Qt::AlignCenter</set>
      </property>
     </widget>
    </item>
    <item>
     <widget class="QPushButton" name="pushButton">
      <property name="text">
       <string>点击我</string>
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

def create_resources_qrc(project_path, project_name):
    """创建 resources.qrc"""
    qrc_content = f'''<!DOCTYPE RCC>
<RCC version="1.0">
    <qresource>
        <!-- 在这里添加资源文件 -->
        <!-- 示例: <file>images/icon.png</file> -->
    </qresource>
    <!-- 注意：Qt 6 的 qt_add_translations 会自动处理翻译文件，无需手动添加到资源文件 -->
    <!-- 如果需要手动管理翻译文件，可以取消下面的注释 -->
    <!--
    <qresource prefix="/i18n">
        <file alias="{project_name}_zh_CN.qm">../translations/{project_name}_zh_CN.qm</file>
    </qresource>
    -->
</RCC>
'''
    qrc_file = project_path / "resources" / "resources.qrc"
    qrc_file.write_text(qrc_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {qrc_file}", Colors.GREEN)

def create_translation_file(project_path, project_name):
    """创建翻译文件模板"""
    ts_content = f'''<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>MainWindow</name>
    <message>
        <source>Qt Application</source>
        <translation>Qt 应用程序</translation>
    </message>
    <message>
        <source>欢迎使用 Qt 应用程序!</source>
        <translation>欢迎使用 Qt 应用程序!</translation>
    </message>
    <message>
        <source>点击我</source>
        <translation>点击我</translation>
    </message>
    <message>
        <source>提示</source>
        <translation>提示</translation>
    </message>
    <message>
        <source>按钮被点击了！\\n这是一个 Qt 应用程序示例。</source>
        <translation>按钮被点击了！\\n这是一个 Qt 应用程序示例。</translation>
    </message>
</context>
</TS>
'''
    ts_file = project_path / "translations" / f"{project_name}_zh_CN.ts"
    ts_file.write_text(ts_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {ts_file}", Colors.GREEN)

def create_gitignore(project_path):
    """创建 .gitignore"""
    gitignore_content = '''# Build directories
build/
cmake-build-*/
*.o
*.a
*.so
*.exe

# CMake
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile

# Qt
*.pro.user
*.pro.user.*
moc_*.cpp
moc_*.h
qrc_*.cpp
ui_*.h
*.autosave
*.qm
*.ts.bak

# IDE
.vscode/settings.json
.idea/
*.swp
*.swo
*~

# OS
.DS_Store
Thumbs.db
'''
    gitignore_file = project_path / ".gitignore"
    gitignore_file.write_text(gitignore_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {gitignore_file}", Colors.GREEN)

def create_readme(project_path, project_name):
    """创建 README.md"""
    readme_content = f'''# {project_name}

Qt C++ 项目

## 项目结构

```
{project_name}/
├── .vscode/              # VSCode 配置文件
│   ├── tasks.json        # 构建任务配置
│   ├── launch.json       # 调试配置
│   ├── c_cpp_properties.json  # IntelliSense 配置
│   └── settings.json     # 工作区设置
├── build/                # 构建输出目录
├── include/              # 头文件目录
│   └── MainWindow.h
├── src/                  # 源代码目录
│   ├── main.cpp
│   └── MainWindow.cpp
├── ui/                   # Qt Designer UI 文件
│   └── MainWindow.ui
├── resources/            # 资源文件目录
│   └── resources.qrc
├── translations/         # 翻译文件目录
│   └── {project_name}_zh_CN.ts
├── CMakeLists.txt        # CMake 构建配置
├── .gitignore
└── README.md
```

## 前置要求

- Qt 5 或 Qt 6（推荐 Qt 6）
- CMake 3.16 或更高版本
- C++17 兼容的编译器（g++/clang++）

### 安装 Qt (Ubuntu/Debian)

```bash
# Qt 6（推荐）
sudo apt-get install qt6-base-dev qt6-base-dev-tools qt6-l10n-tools

# 或 Qt 5
sudo apt-get install qt5-default qtbase5-dev qtbase5-dev-tools
```

## 使用方法

### 在 VSCode 中

1. 打开项目文件夹
2. 按 `Ctrl+Shift+B` 构建项目（CMake 会自动配置）
3. 按 `F5` 运行和调试

### 使用命令行

```bash
# 配置 CMake
mkdir -p build
cd build
cmake ..

# 构建
cmake --build .

# 运行
./bin/{project_name}
```

或者使用一步构建：

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/{project_name}
```

## 开发说明

### 添加新的类

1. 在 `include/` 中创建头文件（如 `MyClass.h`）
2. 在 `src/` 中创建源文件（如 `MyClass.cpp`）
3. 在 `CMakeLists.txt` 中添加文件到 `SOURCES` 和 `HEADERS`

### 使用 UI 文件

项目已配置为使用 Qt Designer UI 文件（`.ui` 文件）：

1. **编辑 UI 文件**：
   - 使用 Qt Designer 打开 `ui/MainWindow.ui` 进行可视化设计
   - 或使用文本编辑器直接编辑 XML 格式的 `.ui` 文件

2. **在代码中访问 UI 控件**：
   ```cpp
   // MainWindow.h 中已包含 Ui::MainWindow 指针
   Ui::MainWindow *ui;
   
   // MainWindow.cpp 中通过 ui-> 访问控件
   ui->pushButton->setText("新文本");
   connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
   ```

3. **添加新的 UI 文件**：
   - 在 `ui/` 目录创建新的 `.ui` 文件
   - 在 `CMakeLists.txt` 的 `UI_FILES` 中添加新文件
   - 在头文件中声明 `namespace Ui {{ class YourClassName; }}`
   - 在源文件中包含 `#include "ui_YourClassName.h"`

4. **IntelliSense 支持**：
   - 首次构建后，`ui_*.h` 文件会在 `build/` 目录自动生成
   - VSCode 的 IntelliSense 会自动识别这些文件（已配置 `build/**` 到 includePath）
   - 如果仍有红色波浪线，执行一次构建（Ctrl+Shift+B）即可

### 添加资源文件

1. 将资源文件（图片、图标等）放入 `resources/` 目录
2. 在 `resources/resources.qrc` 中注册资源文件
3. 在代码中使用 `:/` 前缀访问资源（如 `:/images/icon.png`）

### 国际化支持

项目已配置国际化支持，使用 Qt Linguist 工具：

1. **更新翻译文件**：
   ```bash
   # 更新翻译文件（提取源代码中的 tr() 字符串）
   lupdate src/*.cpp include/*.h ui/*.ui -ts translations/{project_name}_zh_CN.ts
   ```

2. **编辑翻译**：
   - 使用 Qt Linguist 打开 `translations/{project_name}_zh_CN.ts`
   - 或使用文本编辑器直接编辑

3. **编译翻译**：
   ```bash
   # 编译翻译文件为 .qm 格式
   lrelease translations/{project_name}_zh_CN.ts
   ```

4. **添加新语言**：
   - 复制 `translations/{project_name}_zh_CN.ts` 为新语言文件
   - 在 `CMakeLists.txt` 的 `TS_FILES` 中添加新文件
   - 在 `resources.qrc` 中添加对应的 `.qm` 文件

## 跨平台构建

### Linux
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Windows
```bash
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

### macOS/iOS
```bash
# macOS
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build

# iOS (需要设置 iOS SDK)
cmake -B build-ios -S . -DCMAKE_SYSTEM_NAME=iOS
```

### Android
```bash
cmake -B build-android -S . \
  -DCMAKE_SYSTEM_NAME=Android \
  -DCMAKE_ANDROID_NDK=/path/to/android-ndk \
  -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a
```

## 安装

项目已配置安装目标，可以使用以下命令安装：

```bash
# 配置安装路径（可选）
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr/local

# 构建
cmake --build build

# 安装
cmake --install build
```

安装内容包括：
- 可执行文件到 `bin/` 目录
- 翻译文件到 `translations/` 目录
- 库文件到 `lib/` 目录（如果有）

## 调试

1. 在代码中设置断点（点击行号左侧）
2. 按 `F5` 开始调试
3. 使用调试控制面板进行单步调试

## 特性说明

### 现代 CMake 特性
- ✅ 使用 `qt_add_executable()`（Qt 6）或 `add_executable()`（Qt 5）
- ✅ 自动处理 MOC、UIC、RCC
- ✅ PRIVATE 链接方式，符合现代 CMake 最佳实践
- ✅ 支持 Qt 5 和 Qt 6

### UI 文件支持
- ✅ 完整的 Qt Designer UI 文件支持
- ✅ 自动生成 `ui_*.h` 头文件
- ✅ IntelliSense 自动识别生成的 UI 头文件
- ✅ 通过 `ui->` 指针访问 UI 控件

### 国际化
- ✅ 支持多语言翻译
- ✅ 自动加载系统语言
- ✅ 翻译文件集成到资源系统

### 跨平台支持
- ✅ iOS/macOS Bundle 配置
- ✅ Windows 可执行文件配置
- ✅ Android 支持（条件编译）

## 注意事项

- 确保已安装 Qt 开发库和 CMake
- 如果使用 Qt Designer，需要安装 `qt6-tools-dev` 或 `qttools5-dev`
- 首次构建前需要运行 CMake 配置
- Qt 6 推荐使用，提供更好的现代 CMake 支持
'''
    readme_file = project_path / "README.md"
    readme_file.write_text(readme_content, encoding='utf-8')
    print_colored(f"✓ 创建文件: {readme_file}", Colors.GREEN)

def main():
    print_colored("=" * 40, Colors.BLUE)
    print_colored("  VSCode Qt C++ 项目创建工具", Colors.BLUE)
    print_colored("=" * 40, Colors.BLUE)
    print()
    
    # 获取项目名称
    if len(sys.argv) > 1:
        project_name = sys.argv[1]
    else:
        project_name = input("请输入项目名称: ").strip()
    
    if not project_name:
        print_colored("错误: 项目名称不能为空", Colors.RED)
        sys.exit(1)
    
    print()
    print_colored("正在创建项目结构...", Colors.BLUE)
    print()
    
    # 创建项目结构
    project_path = create_project_structure(project_name)
    
    # 创建 CMake 配置文件
    create_cmakelists(project_path, project_name)
    
    # 创建 VSCode 配置文件
    create_tasks_json(project_path)
    create_launch_json(project_path, project_name)
    create_c_cpp_properties_json(project_path)
    create_settings_json(project_path)
    
    # 创建示例文件
    create_main_cpp(project_path, project_name)
    create_mainwindow_h(project_path)
    create_mainwindow_cpp(project_path)
    create_mainwindow_ui(project_path)
    create_resources_qrc(project_path, project_name)
    create_translation_file(project_path, project_name)
    create_gitignore(project_path)
    create_readme(project_path, project_name)
    
    print()
    print_colored(f"✓ 项目 '{project_name}' 创建成功!", Colors.GREEN)
    print()
    print_colored("项目结构:", Colors.BLUE)
    
    # 显示项目结构
    for root, dirs, files in os.walk(project_path):
        level = root.replace(str(project_path), '').count(os.sep)
        indent = ' ' * 2 * level
        print(f"{indent}{os.path.basename(root)}/")
        subindent = ' ' * 2 * (level + 1)
        for file in files:
            print(f"{subindent}{file}")
    
    print()
    print_colored("下一步:", Colors.YELLOW)
    print(f"  1. cd {project_name}")
    print("  2. code .")
    print("  3. 按 Ctrl+Shift+B 构建项目")
    print("  4. 按 F5 运行和调试")
    print()
    print_colored("提示: 如果遇到 Qt 找不到的问题，请确保已安装 Qt6 开发库:", Colors.YELLOW)
    print("  sudo apt-get install qt6-base-dev qt6-base-dev-tools")
    print()

if __name__ == "__main__":
    main()