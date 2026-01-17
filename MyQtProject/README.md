# Ubuntu Qt 开发工具集

一套完整的 Ubuntu 环境下 Qt C++ 开发工具和脚本集合，帮助快速搭建现代化的 Qt 开发环境。

## 📦 包含内容

- **create_qt_project.py**: 自动创建适用于 VSCode 的 Qt 项目模板
- **create_github_actions_ci.py**: 生成 GitHub Actions CI/CD 配置文件，支持跨平台自动构建和发布

## 🚀 快速开始

### 方式 1: 克隆现有仓库

```bash
git clone https://github.com/aosping404/Ubuntu-QT-tools.git
cd Ubuntu-QT-tools
```

### 方式 2: 初始化新仓库

```bash
# 下载或复制文件到本地目录
cd Ubuntu-QT-tools

# 运行初始化脚本
./init_repo.sh

# 或手动初始化
git init
git add .
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/aosping404/Ubuntu-QT-tools.git
git push -u origin main
```

> 💡 详细步骤请查看 [QUICK_START.md](./QUICK_START.md)

### 2. 安装依赖

```bash
# 安装 Qt 开发环境
sudo apt-get install qt6-base-dev qt6-base-dev-tools qt6-l10n-tools qt6-tools-dev

# 安装 Python 依赖（用于工具脚本）
pip install PyQt6
```

### 3. 使用项目创建工具

```bash
# 创建新的 Qt 项目
python3 create_qt_project.py my_project

# 或直接运行，按提示输入项目名称
python3 create_qt_project.py
```

### 4. 生成 CI/CD 配置

```bash
# 使用图形界面生成 GitHub Actions 配置
python3 create_github_actions_ci.py
```

## 📖 详细文档

- **[BLOG.md](./BLOG.md)**: 完整的开发流程和环境配置指南（推荐阅读）
- **[QUICK_START.md](./QUICK_START.md)**: 快速开始指南

## 🛠️ 系统要求

- Ubuntu 20.04+ / Debian 11+ 或其他基于 Debian 的 Linux 发行版
- Qt 6.4+ 或 Qt 5.15+
- CMake 3.16+
- Python 3.8+
- VSCode（推荐）

## 📝 功能特性

### create_qt_project.py

- ✅ 自动创建完整的项目结构
- ✅ 配置 VSCode 开发环境（tasks.json, launch.json, c_cpp_properties.json）
- ✅ 生成 CMakeLists.txt（支持 Qt 5/6）
- ✅ 包含示例代码和 UI 文件
- ✅ 支持国际化（i18n）
- ✅ 自动配置 IntelliSense

### create_github_actions_ci.py

- ✅ 图形化界面配置 CI/CD
- ✅ 支持 Windows、macOS、Linux 多平台构建
- ✅ 自动打包 Qt 依赖
- ✅ 自动发布到 GitHub Release
- ✅ 生成启动脚本，下载即可运行

## 📄 许可证

MIT License

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 📧 联系方式

如有问题或建议，请通过 GitHub Issues 联系。
