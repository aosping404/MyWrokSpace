# qt_qss

Qt C++ 项目

## 项目结构

```
qt_qss/
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
│   └── resources.qrc     # 资源文件（包含 QSS 样式）
├── Styles/               # QSS 样式文件目录
│   ├── *.qss            # 31 个样式主题文件
│   └── README.md        # 样式说明文档
├── translations/         # 翻译文件目录
│   └── qt_qss_zh_CN.ts
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
./bin/qt_qss
```

或者使用一步构建：

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/qt_qss
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
   - 在头文件中声明 `namespace Ui { class YourClassName; }`
   - 在源文件中包含 `#include "ui_YourClassName.h"`

4. **IntelliSense 支持**：
   - 首次构建后，`ui_*.h` 文件会在 `build/` 目录自动生成
   - VSCode 的 IntelliSense 会自动识别这些文件（已配置 `build/**` 到 includePath）
   - 如果仍有红色波浪线，执行一次构建（Ctrl+Shift+B）即可

### 添加资源文件

1. 将资源文件（图片、图标等）放入 `resources/` 目录
2. 在 `resources/resources.qrc` 中注册资源文件
3. 在代码中使用 `:/` 前缀访问资源（如 `:/images/icon.png`）

### QSS 样式支持

项目已集成 31 个精美的 QSS 样式主题，可以直接使用：

#### 可用的样式主题

**深色主题：**
- `ElegantDark` - 优雅深色主题（默认）
- `MaterialDark` - Material Design 深色主题
- `AMOLED` - AMOLED 黑色主题
- `QDarkStyleSheet` - Qt 深色样式表
- `ConsoleStyle` - 控制台风格主题

**浅色主题：**
- `Ubuntu` - Ubuntu 风格主题
- `Aqua` - Aqua 风格主题
- `MacOS` - macOS 风格主题
- `white` - 白色主题

**特色主题：**
- `IKEA` - IKEA 宜家风格主题（基于宜家设计，蓝色主色调 #0058a3，简洁现代，使用 Inter 字体）
- `ManjaroMix` - Manjaro 混合主题（带径向渐变复选框）
- `NeonButtons` - 霓虹按钮主题
- `coffee` - 咖啡色主题
- `khaki` - 卡其色主题
- `blue` - 蓝色主题
- `transparent` - 透明主题
- `pagefold` - 页面折叠风格
- `phonegui` - 手机 GUI 风格

**其他主题：**
- `style`, `style2`, `style3`, `style4` - 多种风格主题
- `stylesheet2`, `or`, `abc`, `abc_satellite` - 其他样式
- `application`, `default`, `default2` - 应用默认样式
- `OverlayWidget` - 覆盖小部件样式
- `nostyle`, `no` - 无样式（空白）

#### 使用方法

1. **在代码中加载样式**：
   ```cpp
   // MainWindow.cpp 中已包含 loadStyleSheet 方法
   // 加载指定样式（从资源文件）
   loadStyleSheet("ElegantDark");
   
   // 或加载其他样式
   loadStyleSheet("IKEA");        // IKEA 宜家风格主题
   loadStyleSheet("MaterialDark");
   loadStyleSheet("Ubuntu");
   loadStyleSheet("AMOLED");
   ```

2. **从资源文件加载**：
   所有 QSS 文件已添加到 `resources.qrc` 中，使用 `:/styles/样式名.qss` 访问：
   ```cpp
   QFile file(":/styles/ElegantDark.qss");
   if (file.open(QFile::ReadOnly | QFile::Text)) {
       QTextStream stream(&file);
       QString styleSheet = stream.readAll();
       qApp->setStyleSheet(styleSheet);
   }
   ```

3. **从文件系统加载**：
   如果资源文件加载失败，代码会自动尝试从 `Styles/` 目录加载：
   ```cpp
   QFile file("Styles/ElegantDark.qss");
   // ... 加载代码
   ```

4. **动态切换样式**：
   可以在运行时切换不同的样式主题：
   ```cpp
   // 例如：在菜单或按钮点击时切换
   void MainWindow::onStyleChanged(const QString &styleName) {
       loadStyleSheet(styleName);
   }
   ```

#### 样式文件位置

- 样式文件位于 `Styles/` 目录
- 所有样式文件已注册到资源文件 `resources/resources.qrc`
- 资源路径前缀：`:/styles/`

#### 查看样式预览

查看 `Styles/README.md` 了解各主题的详细信息和预览截图。

### 国际化支持

项目已配置国际化支持，使用 Qt Linguist 工具：

1. **更新翻译文件**：
   ```bash
   # 更新翻译文件（提取源代码中的 tr() 字符串）
   lupdate src/*.cpp include/*.h ui/*.ui -ts translations/qt_qss_zh_CN.ts
   ```

2. **编辑翻译**：
   - 使用 Qt Linguist 打开 `translations/qt_qss_zh_CN.ts`
   - 或使用文本编辑器直接编辑

3. **编译翻译**：
   ```bash
   # 编译翻译文件为 .qm 格式
   lrelease translations/qt_qss_zh_CN.ts
   ```

4. **添加新语言**：
   - 复制 `translations/qt_qss_zh_CN.ts` 为新语言文件
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
cmake -B build-android -S .   -DCMAKE_SYSTEM_NAME=Android   -DCMAKE_ANDROID_NDK=/path/to/android-ndk   -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a
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
