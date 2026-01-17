# Ubuntu 环境下 Qt C++ 开发完整指南

## 目录

1. [系统环境介绍](#系统环境介绍)
2. [Qt 开发环境配置](#qt-开发环境配置)
3. [VSCode 插件配置](#vscode-插件配置)
4. [使用 create_qt_project 创建项目](#使用-create_qt_project-创建项目)
5. [Qt Creator 与 VSCode 协同开发](#qt-creator-与-vscode-协同开发)
6. [GitHub Actions CI/CD 配置](#github-actions-cicd-配置)
7. [开发工作流](#开发工作流)

---

## 系统环境介绍

### 操作系统

- **发行版**: Ubuntu 24.04.3 LTS (Noble)
- **内核版本**: Linux 6.14.0-37-generic
- **架构**: x86_64

### 开发工具版本

```bash
# Qt 版本
Qt 6.4.2
QMake version 3.1

# 构建工具
CMake 3.28.3
g++ (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0

# 编辑器
VSCode (通过 /usr/bin/code 访问)
```

### 为什么选择这个环境？

- **Ubuntu 24.04 LTS**: 长期支持版本，稳定性好，软件包更新及时
- **Qt 6.4.2**: 现代 C++ 特性支持，更好的 CMake 集成
- **CMake 3.28**: 最新稳定版，支持现代 CMake 最佳实践
- **VSCode**: 轻量级、插件丰富、跨平台

---

## Qt 开发环境配置

### 1. 安装 Qt 开发库

```bash
# 安装 Qt 6 核心库和开发工具
sudo apt-get update
sudo apt-get install -y \
    qt6-base-dev \
    qt6-base-dev-tools \
    qt6-l10n-tools \
    qt6-tools-dev \
    qt6-tools-dev-tools

# 验证安装
qmake6 --version
```

**安装内容说明**:
- `qt6-base-dev`: Qt 6 核心开发库（Core, Widgets, Gui 等）
- `qt6-base-dev-tools`: Qt 6 基础开发工具（qmake, moc, uic, rcc）
- `qt6-l10n-tools`: 国际化工具（lupdate, lrelease）
- `qt6-tools-dev`: Qt Designer 和 Qt Linguist
- `qt6-tools-dev-tools`: Qt Creator（可选，用于 UI 设计）

### 2. 安装 CMake 和编译器

```bash
# 安装 CMake
sudo apt-get install -y cmake

# 安装 C++ 编译器和构建工具
sudo apt-get install -y \
    build-essential \
    g++ \
    gdb

# 验证安装
cmake --version
g++ --version
```

### 3. 安装 Python 依赖（用于工具脚本）

```bash
# 安装 PyQt6（用于 create_github_actions_ci.py 的图形界面）
pip install PyQt6

# 或使用 conda
conda install pyqt
```

### 4. 环境变量配置（可选）

```bash
# 添加到 ~/.bashrc 或 ~/.zshrc
export Qt6_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6
export PATH=/usr/lib/qt6/bin:$PATH
```

---

## VSCode 插件配置

### 必需的插件

以下是我使用的 VSCode 插件列表，按重要性排序：

#### 1. C/C++ 开发插件

```bash
# 核心 C++ 支持
ms-vscode.cpptools                    # C/C++ 扩展包
ms-vscode.cpptools-extension-pack     # C/C++ 扩展包集合
ms-vscode.cpptools-themes             # C/C++ 主题

# CMake 支持
ms-vscode.cmake-tools                 # CMake 工具（必需！）
ms-vscode.makefile-tools              # Makefile 工具
```

**功能**:
- IntelliSense 代码补全
- 代码导航和跳转
- 调试支持
- CMake 项目配置和构建

#### 2. Qt 专用插件

```bash
# Qt 官方插件
theqtcompany.qt-core                  # Qt Core 支持
theqtcompany.qt-cpp                   # Qt C++ 支持
theqtcompany.qt-ui                    # Qt UI 文件支持
```

**功能**:
- Qt 类和方法补全
- `.ui` 文件语法高亮
- `.qrc` 资源文件支持
- Qt 信号槽连接提示

#### 3. 辅助插件

```bash
# 代码质量
aaron-bond.better-comments            # 更好的注释高亮
andrzejzwierzchowski.al-code-outline # 代码大纲
kisstkondoros.vscode-gutter-preview  # 代码预览

# 界面美化
emmanuelbeziat.vscode-great-icons     # 文件图标
naumovs.color-highlight               # 颜色高亮
oderwat.indent-rainbow                # 缩进彩虹
shalldie.background                   # 背景图片

# AI 辅助（可选）
github.copilot                         # GitHub Copilot
github.copilot-chat                   # Copilot Chat

# 其他
ms-ceintl.vscode-language-pack-zh-hans # 中文语言包
siyam.codeify                          # 代码格式化
```

### 插件安装方法

#### 方法 1: 通过命令行安装

```bash
# 安装单个插件
code --install-extension ms-vscode.cmake-tools
code --install-extension theqtcompany.qt-core
code --install-extension theqtcompany.qt-cpp
code --install-extension theqtcompany.qt-ui

# 批量安装（创建 extensions.txt）
cat > extensions.txt << EOF
ms-vscode.cmake-tools
ms-vscode.cpptools
theqtcompany.qt-core
theqtcompany.qt-cpp
theqtcompany.qt-ui
EOF

# 安装列表中的所有插件
cat extensions.txt | xargs -L 1 code --install-extension
```

#### 方法 2: 通过 VSCode 界面安装

1. 打开 VSCode
2. 点击左侧扩展图标（或按 `Ctrl+Shift+X`）
3. 搜索插件名称
4. 点击"安装"

### VSCode 配置建议

创建或编辑 `~/.config/Code/User/settings.json`:

```json
{
  "cmake.configureOnOpen": true,
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "files.associations": {
    "*.ui": "xml",
    "*.qrc": "xml",
    "*.qml": "qml"
  },
  "C_Cpp.default.intelliSenseMode": "linux-gcc-x64",
  "C_Cpp.default.compilerPath": "/usr/bin/g++",
  "C_Cpp.default.cppStandard": "c++17"
}
```

---

## 使用 create_qt_project 创建项目

### 工具介绍

`create_qt_project.py` 是一个 Python 脚本，用于自动创建完整的 Qt 项目结构，包括：
- 标准的项目目录结构
- CMakeLists.txt 配置
- VSCode 配置文件
- 示例代码和 UI 文件
- 国际化支持

### 使用方法

#### 1. 基本使用

```bash
# 方式 1: 直接运行，按提示输入项目名称
python3 create_qt_project.py

# 方式 2: 通过命令行参数指定项目名称
python3 create_qt_project.py my_qt_app
```

#### 2. 生成的项目结构

```
my_qt_app/
├── .vscode/                    # VSCode 配置
│   ├── tasks.json              # 构建任务
│   ├── launch.json             # 调试配置
│   ├── c_cpp_properties.json   # IntelliSense 配置
│   └── settings.json            # 工作区设置
├── build/                      # 构建输出目录
├── include/                    # 头文件
│   └── MainWindow.h
├── src/                        # 源代码
│   ├── main.cpp
│   └── MainWindow.cpp
├── ui/                         # Qt Designer UI 文件
│   └── MainWindow.ui
├── resources/                  # 资源文件
│   └── resources.qrc
├── translations/               # 翻译文件
│   └── my_qt_app_zh_CN.ts
├── CMakeLists.txt              # CMake 配置
├── .gitignore
└── README.md
```

#### 3. 项目特性

**CMakeLists.txt 特性**:
- ✅ 支持 Qt 5 和 Qt 6
- ✅ 自动处理 MOC、UIC、RCC
- ✅ 自动生成 `ui_*.h` 文件
- ✅ 国际化支持
- ✅ 跨平台配置（Windows、macOS、Linux）

**VSCode 配置特性**:
- ✅ 一键构建（Ctrl+Shift+B）
- ✅ 一键调试（F5）
- ✅ IntelliSense 自动识别生成的 UI 头文件
- ✅ CMake 自动配置

#### 4. 开始开发

```bash
# 进入项目目录
cd my_qt_app

# 使用 VSCode 打开
code .

# 在 VSCode 中：
# 1. 按 Ctrl+Shift+B 构建项目
# 2. 按 F5 运行和调试
```

---

## Qt Creator 与 VSCode 协同开发

### 为什么需要协同？

- **Qt Creator**: 优秀的 UI 设计工具（Qt Designer），可视化编辑 `.ui` 文件
- **VSCode**: 更好的代码编辑体验，丰富的插件生态

### 工作流程

#### 1. 在 VSCode 中编写代码

```bash
# 使用 VSCode 打开项目
code my_qt_app
```

在 VSCode 中：
- 编写 C++ 代码（`src/` 和 `include/`）
- 使用 IntelliSense 获得代码补全
- 使用调试功能（F5）
- 使用 Git 进行版本控制

#### 2. 在 Qt Creator 中设计 UI

```bash
# 方式 1: 直接打开 .ui 文件
qtcreator ui/MainWindow.ui

# 方式 2: 打开整个项目
qtcreator my_qt_app/CMakeLists.txt
```

在 Qt Creator 中：
- 使用 Qt Designer 可视化编辑 `.ui` 文件
- 拖拽控件，设置属性
- 预览界面效果
- 保存后，`.ui` 文件会自动更新

#### 3. 同步工作

**重要**: `.ui` 文件是 XML 格式，两个编辑器都可以编辑：

- **Qt Creator**: 可视化编辑，适合复杂布局
- **VSCode**: 文本编辑，适合快速修改属性

**工作流建议**:
1. 在 Qt Creator 中设计主要 UI 布局
2. 在 VSCode 中编写业务逻辑代码
3. 需要调整 UI 时，回到 Qt Creator 或直接在 VSCode 中编辑 XML

### 配置 Qt Creator

1. **打开 Qt Creator**
2. **工具 → 选项 → Kits**
   - 确保配置了正确的 Qt 版本
   - 确保配置了 CMake

3. **打开项目**
   - 文件 → 打开文件或项目
   - 选择项目的 `CMakeLists.txt`

4. **配置构建**
   - 选择构建目录（建议使用 `build/`）
   - 选择构建类型（Debug/Release）

### 注意事项

- **不要同时编辑同一个文件**: 虽然技术上可行，但可能导致冲突
- **保存顺序**: 在 Qt Creator 中保存 `.ui` 后，VSCode 会自动检测到变化
- **构建**: 可以在任一编辑器中构建，但建议统一使用 VSCode 的构建任务

---

## GitHub Actions CI/CD 配置

### 工具介绍

`create_github_actions_ci.py` 是一个图形化工具，用于生成 GitHub Actions CI/CD 配置文件，支持：
- 跨平台自动构建（Windows、macOS、Linux）
- 自动打包 Qt 依赖
- 自动发布到 GitHub Release
- 生成可执行的发布包

### 使用方法

#### 1. 运行工具

```bash
python3 create_github_actions_ci.py
```

#### 2. 配置选项

图形界面包含以下配置项：

**项目信息**:
- 项目名称
- Qt 版本（6.8.3, 6.8.0, 6.7.0 等）
- Python 版本（默认 3.10）
- 构建类型（Debug/Release）

**支持平台**:
- ✅ Windows (windows-latest)
- ✅ macOS (macos-latest)
- ✅ Ubuntu (ubuntu-latest)

**触发条件**:
- Push 代码时触发
- Pull Request 时触发
- 发布标签时触发（v*）
- 手动触发

**额外选项**:
- 上传构建产物（Artifacts）
- 启用 Qt 缓存（加速构建）

#### 3. 生成配置文件

1. 填写项目信息
2. 选择支持的平台
3. 选择触发条件
4. 点击"生成配置文件"
5. 配置文件会保存到 `.github/workflows/ci.yml`

#### 4. CI/CD 功能

**自动构建**:
- 在每次 Push 或 PR 时自动构建
- 支持多平台并行构建
- 自动检测和打包 Qt 依赖

**自动发布**:
- 当推送 `v*` 标签时（如 `v1.0.0`）
- 自动创建 GitHub Release
- 上传所有平台的构建包
- 生成 Release 说明

**打包特性**:
- **Windows**: 使用 `windeployqt` 打包所有依赖，生成 `.exe` 和启动脚本
- **macOS**: 使用 `macdeployqt` 创建 `.app` bundle
- **Linux**: 自动复制 Qt 库和插件，生成启动脚本

#### 5. 使用示例

```bash
# 1. 提交代码
git add .
git commit -m "Add new feature"
git push

# 2. 创建标签并发布
git tag v1.0.0
git push origin v1.0.0

# 3. GitHub Actions 会自动：
#    - 在所有平台构建项目
#    - 打包所有依赖
#    - 创建 Release 页面
#    - 上传构建包
```

---

## 开发工作流

### 完整开发流程

#### 1. 创建新项目

```bash
# 使用工具创建项目
python3 create_qt_project.py my_app

# 进入项目目录
cd my_app

# 使用 VSCode 打开
code .
```

#### 2. 设计 UI（可选）

```bash
# 在 Qt Creator 中打开 UI 文件
qtcreator ui/MainWindow.ui

# 或直接在 VSCode 中编辑 XML
code ui/MainWindow.ui
```

#### 3. 编写代码

在 VSCode 中：
- 编辑 `src/` 和 `include/` 中的代码
- 使用 IntelliSense 获得代码补全
- 使用 Git 进行版本控制

#### 4. 构建和调试

```bash
# 方式 1: 使用 VSCode 快捷键
# Ctrl+Shift+B: 构建
# F5: 运行和调试

# 方式 2: 使用命令行
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/my_app
```

#### 5. 配置 CI/CD（可选）

```bash
# 生成 CI/CD 配置
python3 create_github_actions_ci.py

# 提交配置
git add .github/workflows/ci.yml
git commit -m "Add CI/CD configuration"
git push
```

#### 6. 发布版本

```bash
# 创建标签
git tag v1.0.0
git push origin v1.0.0

# GitHub Actions 会自动构建和发布
```

### 日常开发建议

1. **代码编辑**: 主要在 VSCode 中进行
2. **UI 设计**: 复杂布局使用 Qt Creator，简单修改直接在 VSCode 中编辑 XML
3. **构建测试**: 使用 VSCode 的构建任务（Ctrl+Shift+B）
4. **调试**: 使用 VSCode 的调试功能（F5）
5. **版本控制**: 使用 VSCode 的 Git 集成或命令行

### 常见问题

#### Q: IntelliSense 找不到 Qt 头文件？

**A**: 确保：
1. 已安装 `qt6-base-dev`
2. 已运行一次构建（生成 `compile_commands.json`）
3. VSCode 的 C/C++ 插件已正确配置

#### Q: UI 文件修改后代码中找不到控件？

**A**: 
1. 重新构建项目（Ctrl+Shift+B）
2. 确保 `ui_*.h` 文件已生成在 `build/` 目录
3. 检查 `CMakeLists.txt` 中的 `UI_FILES` 配置

#### Q: 如何添加新的 UI 文件？

**A**:
1. 在 `ui/` 目录创建新的 `.ui` 文件
2. 在 `CMakeLists.txt` 的 `UI_FILES` 中添加
3. 在头文件中声明 `namespace Ui { class YourClassName; }`
4. 在源文件中包含 `#include "ui_YourClassName.h"`

#### Q: GitHub Actions 构建失败？

**A**: 检查：
1. `.github/workflows/ci.yml` 配置是否正确
2. 项目名称是否匹配
3. CMakeLists.txt 是否正确
4. 查看 GitHub Actions 日志获取详细错误信息

---

## 总结

本文介绍了在 Ubuntu 环境下进行 Qt C++ 开发的完整流程：

1. ✅ **环境配置**: Qt 6、CMake、编译器
2. ✅ **编辑器配置**: VSCode 插件和设置
3. ✅ **项目创建**: 使用自动化工具快速创建项目
4. ✅ **协同开发**: Qt Creator 和 VSCode 的配合使用
5. ✅ **CI/CD**: 自动化构建和发布

这套工具链的优势：
- 🚀 **快速启动**: 几分钟内创建完整的项目结构
- 🎯 **开箱即用**: 所有配置已优化，无需手动调整
- 🔧 **现代化**: 使用最新的 CMake 和 Qt 6 特性
- 🌍 **跨平台**: 支持 Windows、macOS、Linux
- 🤖 **自动化**: CI/CD 自动构建和发布

希望这个指南能帮助你快速上手 Qt 开发！

---

## 相关资源

- [Qt 官方文档](https://doc.qt.io/)
- [CMake 文档](https://cmake.org/documentation/)
- [VSCode C++ 扩展文档](https://code.visualstudio.com/docs/languages/cpp)
- [GitHub Actions 文档](https://docs.github.com/en/actions)

## 许可证

MIT License

---

**作者**: aosping404  
**最后更新**: 2024年
