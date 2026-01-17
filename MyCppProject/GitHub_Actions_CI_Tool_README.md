# GitHub Actions CI/CD 配置生成工具

一个使用 PyQt 创建的可视化工具,用于生成跨平台的 GitHub Actions CI/CD 配置文件。

## 功能特性

- ✅ **可视化界面**: 使用 PyQt 创建友好的图形界面
- ✅ **跨平台支持**: 生成支持 Windows、macOS 和 Ubuntu 的 CI/CD 配置
- ✅ **Qt 环境配置**: 自动使用 `jurplel/install-qt-action` 插件配置 Qt 环境
- ✅ **灵活配置**: 支持自定义 Qt 版本、Python 版本、构建类型等
- ✅ **多种触发条件**: 支持 push、pull_request、标签发布、手动触发等
- ✅ **构建产物上传**: 自动配置 Artifacts 上传
- ✅ **Qt 缓存**: 支持启用 Qt 缓存以加速构建

## 安装要求

### Python 环境
- Python 3.8 或更高版本

### 方法一: 使用 Conda (推荐 - 隔离环境)

Conda 可以创建一个隔离的 Python 环境,避免与系统 Python 环境冲突。

#### 1. 安装 Conda

如果您还没有安装 Conda,可以从以下地址下载:
- **Miniconda**: https://docs.conda.io/en/latest/miniconda.html (推荐,体积小)
- **Anaconda**: https://www.anaconda.com/products/distribution (完整版,包含更多工具)

#### 2. 创建 Conda 环境

**方法 A: 使用自动脚本 (推荐)**

```bash
# 给脚本添加执行权限
chmod +x conda_setup.sh

# 运行脚本创建环境
./conda_setup.sh
```

**方法 B: 手动创建环境**

```bash
# 使用 environment.yml 文件创建环境
conda env create -f environment.yml

# 激活环境
conda activate github-actions-ci-tool
```

#### 3. 使用环境

```bash
# 激活环境
conda activate github-actions-ci-tool

# 运行工具
python create_github_actions_ci.py

# 退出环境
conda deactivate
```

#### 4. 管理环境

```bash
# 列出所有 conda 环境
conda env list

# 更新环境 (如果修改了 environment.yml)
conda env update -f environment.yml --prune

# 删除环境 (如果不再需要)
conda env remove -n github-actions-ci-tool
```

### 方法二: 使用 pip (系统 Python)

#### 安装依赖

```bash
# 安装 PyQt6
pip install PyQt6>=6.0.0

# 或使用 requirements.txt
pip install -r github_actions_tool_requirements.txt
```

#### Ubuntu/Debian 系统额外依赖

```bash
# 安装系统依赖
sudo apt-get install python3-pyqt6
# 或
sudo apt-get install python3-qt6
```

### 方法三: 使用虚拟环境 (venv)

```bash
# 创建虚拟环境
python3 -m venv venv

# 激活虚拟环境 (Linux/macOS)
source venv/bin/activate

# 激活虚拟环境 (Windows)
# venv\Scripts\activate

# 安装依赖
pip install -r github_actions_tool_requirements.txt

# 运行工具
python create_github_actions_ci.py
```

## 使用方法

### 启动应用程序

```bash
python3 create_github_actions_ci.py
```

### 操作步骤

1. **填写项目信息**
   - 项目名称: 您的项目名称
   - Qt 版本: 选择需要的 Qt 版本 (默认 6.8.3)
   - Python 版本: 输入 Python 版本 (默认 3.10)
   - 构建类型: 选择 Debug 或 Release

2. **选择支持平台**
   - ☑ Windows (windows-latest)
   - ☑ macOS (macos-latest)
   - ☑ Ubuntu (ubuntu-latest)

3. **配置触发条件**
   - ☑ 推送代码时触发 (push)
   - ☑ Pull Request 时触发 (pull_request)
   - ☐ 发布标签时触发 (tags: 'v*')
   - ☐ 手动触发 (workflow_dispatch)

4. **设置额外选项**
   - ☑ 上传构建产物 (Artifacts)
   - ☑ 启用 Qt 缓存 (加速构建)

5. **选择输出目录**
   - 默认: `.github/workflows`
   - 可以点击"浏览..."按钮选择其他目录

6. **生成配置文件**
   - 点击"预览配置"查看生成的 YAML 内容
   - 点击"生成配置文件"保存到指定目录

## 生成的配置文件示例

生成的 `.github/workflows/ci.yml` 文件示例:

```yaml
name: CI / Build / Package

on:
  push:
  pull_request:
    branches: [ main, master ]

jobs:
  build:
    name: Build on ${{ matrix.os }}
    runs-on: ${{ matrix.os }}
    strategy:
      fail-fast: false
      matrix:
        os:
        - windows-latest
        - macos-latest
        - ubuntu-latest
        host:
        - windows
        - mac
        - linux

    steps:
    - name: Checkout code
      uses: actions/checkout@v4

    - name: Setup Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.10'

    - name: Install Qt
      uses: jurplel/install-qt-action@v4
      with:
        version: '6.8.3'
        host: ${{ matrix.host }}
        target: desktop
        modules: 'qtbase qttools qtsvg qtdeclarative'
        cache: true

    - name: Setup CMake
      uses: jwlawson/actions-setup-cmake@v1.14
      with:
        cmake-version: '3.27'

    - name: Configure CMake
      run: |
        cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
      env:
        Qt6_DIR: ${{ env.Qt6_DIR }}

    - name: Build
      run: |
        cmake --build build --config Release

    - name: Upload build artifacts
      uses: actions/upload-artifact@v4
      with:
        name: my_qt_project-${{ matrix.os }}-${{ github.run_number }}
        path: |
          build/bin/**
          build/**/*.exe
          build/**/*.app
        if-no-files-found: warn
```

## 使用生成的配置

1. **提交到 Git 仓库**
   ```bash
   git add .github/workflows/ci.yml
   git commit -m "添加 GitHub Actions CI/CD 配置"
   git push
   ```

2. **在 GitHub 上查看**
   - 进入仓库的 "Actions" 标签页
   - 查看构建状态和结果
   - 下载构建产物 (如果启用)

## 核心工具说明

### jurplel/install-qt-action

这是生成配置的核心工具,它能够:

- 🚀 **快速配置**: 在几分钟内配置好三个系统的 Qt 环境
- 🔧 **自动安装**: 自动安装正确版本的 Qt 和所需模块
- 💾 **缓存支持**: 支持缓存以加速后续构建
- 🎯 **精确控制**: 支持指定版本、主机、目标平台、架构等

### GitHub Actions

GitHub Actions 是 GitHub 提供的 CI/CD 服务,能够:

- 🔄 **自动构建**: 提交代码时自动触发构建
- 🏗️ **跨平台**: 在真实的 Windows、macOS 和 Ubuntu 环境中构建
- 📦 **自动打包**: 自动打包并上传构建产物
- ✅ **持续集成**: 持续集成和持续部署的最佳实践

## 项目结构

```
MyCppProject/
├── create_github_actions_ci.py          # 主应用程序
├── github_actions_tool_requirements.txt # Python 依赖 (pip)
├── environment.yml                       # Conda 环境配置
├── conda_setup.sh                        # Conda 环境自动设置脚本
└── GitHub_Actions_CI_Tool_README.md     # 本说明文档
```

## 常见问题

### Q: 如何修改生成的配置文件?

A: 可以直接编辑生成的 `.github/workflows/ci.yml` 文件,或者重新运行工具生成新的配置。

### Q: 生成的配置是否适用于所有 Qt 项目?

A: 生成的配置是针对基于 CMake 的 Qt 项目的通用配置。如果您的项目使用其他构建系统 (如 qmake),可能需要手动调整。

### Q: 如何添加更多构建步骤?

A: 可以在"预览配置"中查看生成的 YAML,然后手动编辑添加额外的步骤。

### Q: 是否支持其他 CI/CD 平台?

A: 目前仅支持 GitHub Actions。其他平台 (如 GitLab CI、Jenkins 等) 的配置文件生成功能可以在未来版本中添加。

## 技术栈

- **GUI 框架**: PyQt6
- **CI/CD 平台**: GitHub Actions
- **Qt 安装工具**: jurplel/install-qt-action
- **构建系统**: CMake

## 许可证

本项目遵循与主项目相同的许可证。

## 贡献

欢迎提交 Issue 和 Pull Request!

## 更新日志

### v1.0.0 (2024)
- ✨ 初始版本
- ✨ 支持生成 GitHub Actions CI/CD 配置
- ✨ 可视化界面配置
- ✨ 跨平台支持 (Windows、macOS、Ubuntu)

---

**提示**: 使用此工具生成的配置文件需要根据您的具体项目需求进行调整。建议在提交到仓库前仔细检查配置内容。