# qt_ycx_Virtual_function_inheritance

Qt C++ 项目

## 项目结构

```
qt_ycx_Virtual_function_inheritance/
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
├── CMakeLists.txt        # CMake 构建配置
├── .gitignore
└── README.md
```

## 前置要求

- Qt 5 或 Qt 6
- CMake 3.16 或更高版本
- C++17 兼容的编译器（g++/clang++）

### 安装 Qt (Ubuntu/Debian)

```bash
# Qt 6
sudo apt-get install qt6-base-dev qt6-base-dev-tools

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
./bin/qt_ycx_Virtual_function_inheritance
```

或者使用一步构建：

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/qt_ycx_Virtual_function_inheritance
```

## 开发说明

### 添加新的类

1. 在 `include/` 中创建头文件（如 `MyClass.h`）
2. 在 `src/` 中创建源文件（如 `MyClass.cpp`）
3. 在 `CMakeLists.txt` 中添加文件到 `SOURCES` 和 `HEADERS`

### 添加 UI 文件

1. 使用 Qt Designer 创建 `.ui` 文件并保存到 `ui/` 目录
2. 在 `CMakeLists.txt` 中添加文件到 `UI_FILES`
3. 在代码中使用 `ui->` 访问 UI 控件

### 添加资源文件

1. 将资源文件（图片、图标等）放入 `resources/` 目录
2. 在 `resources/resources.qrc` 中注册资源文件
3. 在代码中使用 `:/` 前缀访问资源（如 `:/images/icon.png`）

## 调试

1. 在代码中设置断点（点击行号左侧）
2. 按 `F5` 开始调试
3. 使用调试控制面板进行单步调试

## 注意事项

- 确保已安装 Qt 开发库和 CMake
- 如果使用 Qt Designer，需要安装 `qt6-tools-dev` 或 `qttools5-dev`
- 首次构建前需要运行 CMake 配置
