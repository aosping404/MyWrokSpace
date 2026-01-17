# 快速开始

## 创建新项目

### 方法一：Python 脚本（推荐）

```bash
# 交互式
./create_cpp_project.py

# 直接指定项目名称
./create_cpp_project.py classTest
```

### 方法二：Bash 脚本

```bash
# 交互式
./create_cpp_project.sh

# 直接指定项目名称
./create_cpp_project.sh classTest
```

## 使用示例

```bash
# 1. 创建项目
./create_cpp_project.py classTest

# 2. 进入项目目录
cd classTest

# 3. 在 VSCode 中打开
code .

# 4. 在 VSCode 中：
#    - 按 F5 运行/调试
#    - 按 Ctrl+Shift+B 构建
#    - 点击行号设置断点
```

## 项目结构

```
项目名称/
├── .vscode/          # VSCode 配置（已自动配置好）
├── include/          # 头文件
├── src/              # 源代码
└── README.md         # 项目说明
```

## 功能特性

✅ 自动创建完整的 VSCode C++ 项目结构  
✅ 配置好构建、调试和 IntelliSense  
✅ 包含示例代码  
✅ 支持命令行参数和交互式两种方式  
✅ 符合 Ubuntu/Linux 环境

