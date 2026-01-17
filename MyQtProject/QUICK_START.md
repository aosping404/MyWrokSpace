# 快速开始指南

## 1. 初始化 Git 仓库

### 首次创建仓库

```bash
# 在 Ubuntu-QT-tools 目录下
echo "# Ubuntu-QT-tools" >> README.md
git init
git add README.md
git branch -M main
git remote add origin https://github.com/aosping404/Ubuntu-QT-tools.git
git push -u origin main
```

### 从现有仓库推送

```bash
git remote add origin https://github.com/aosping404/Ubuntu-QT-tools.git
git branch -M main
git push -u origin main
```

## 2. 安装依赖

```bash
# 安装 Qt 开发环境
sudo apt-get update
sudo apt-get install -y \
    qt6-base-dev \
    qt6-base-dev-tools \
    qt6-l10n-tools \
    qt6-tools-dev

# 安装 Python 依赖
pip install -r requirements.txt
```

## 3. 使用工具

### 创建 Qt 项目

```bash
python3 create_qt_project.py my_project
cd my_project
code .
```

### 生成 CI/CD 配置

```bash
python3 create_github_actions_ci.py
```

## 4. 查看完整文档

详细的使用说明请查看 [BLOG.md](./BLOG.md)
