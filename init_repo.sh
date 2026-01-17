#!/bin/bash
# MyWorkSpace Git 仓库初始化脚本

set -e

echo "=========================================="
echo "  MyWorkSpace Git 仓库初始化"
echo "=========================================="
echo ""

# 检查是否已经是 Git 仓库
if [ -d ".git" ]; then
    echo "⚠️  当前目录已经是 Git 仓库"
    read -p "是否继续初始化？(y/n): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "操作已取消"
        exit 1
    fi
fi

# 检查 .gitignore 是否存在
if [ ! -f ".gitignore" ]; then
    echo "⚠️  未找到 .gitignore 文件，将创建默认的 .gitignore"
    echo "请确保已排除模型文件和敏感文件"
fi

# 检查 README.md 是否存在
if [ ! -f "README.md" ]; then
    echo "创建 README.md..."
    cat > README.md <<EOF
# MyWorkSpace

个人工作空间 - 包含 C++、Qt、YOLO 深度学习等项目

## 项目结构

- **MyCppProject/** - C++ 项目集合
- **MyQtProject/** - Qt 应用程序项目集合
- **yolo_project/** - YOLO 深度学习项目
- **ROSProject/** - ROS2 机器人项目（待开发）

## 文档

- [工作空间结构说明](工作空间结构说明.md)
- [系统环境信息汇总](系统环境信息汇总.md)
- [项目环境脚本使用说明](project_env_使用说明.md)
- [YOLO项目环境切换说明](YOLO项目环境切换说明.md)

## 环境管理

使用 \`project_env.sh\` 脚本管理不同项目的环境：

\`\`\`bash
source project_env.sh yolo  # 切换到 YOLO 项目环境
source project_env.sh default  # 切换到默认环境
\`\`\`

详见 [project_env_使用说明.md](project_env_使用说明.md)

## License

MIT License
EOF
    echo "✓ README.md 已创建"
fi

# 初始化 Git 仓库
if [ ! -d ".git" ]; then
    echo "初始化 Git 仓库..."
    git init
    echo "✓ Git 仓库初始化完成"
fi

# 检查是否有远程仓库
if git remote | grep -q "^origin$"; then
    echo "⚠️  远程仓库 'origin' 已存在"
    CURRENT_URL=$(git remote get-url origin)
    echo "当前远程仓库地址: $CURRENT_URL"
    read -p "是否更新远程仓库地址？(y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        git remote set-url origin https://github.com/aosping404/MyWrokSpace.git
        echo "✓ 远程仓库地址已更新为: https://github.com/aosping404/MyWrokSpace.git"
    fi
else
    echo "添加远程仓库..."
    git remote add origin https://github.com/aosping404/MyWrokSpace.git
    echo "✓ 远程仓库已添加: https://github.com/aosping404/MyWrokSpace.git"
fi

# 设置主分支
echo "设置主分支为 main..."
git branch -M main

# 添加文件（排除 .gitignore 中的文件）
echo "添加文件到 Git（排除模型文件和敏感文件）..."
git add .

# 检查是否有需要提交的文件
if git diff --cached --quiet; then
    echo "⚠️  没有需要提交的文件"
else
    # 显示将要提交的文件（前20个）
    echo ""
    echo "将要提交的文件（前20个）:"
    git diff --cached --name-only | head -20
    FILE_COUNT=$(git diff --cached --name-only | wc -l)
    if [ "$FILE_COUNT" -gt 20 ]; then
        echo "... 还有 $((FILE_COUNT - 20)) 个文件"
    fi
    echo ""
    
    # 首次提交
    if [ -z "$(git log --oneline 2>/dev/null)" ]; then
        echo "创建首次提交..."
        git commit -m "first commit: Add MyWorkSpace projects and documentation"
        echo "✓ 首次提交完成"
    else
        read -p "是否提交更改？(y/n): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            read -p "输入提交信息 (默认: Update workspace): " COMMIT_MSG
            COMMIT_MSG=${COMMIT_MSG:-"Update workspace"}
            git commit -m "$COMMIT_MSG"
            echo "✓ 提交完成"
        fi
    fi
fi

echo ""
echo "=========================================="
echo "  初始化完成！"
echo "=========================================="
echo ""
echo "下一步："
echo "  1. 检查文件: git status"
echo "  2. 查看将要排除的文件: git check-ignore -v <file>"
echo "  3. 推送到远程: git push -u origin main"
echo ""
echo "注意: 模型文件 (.pt) 和敏感文件已被 .gitignore 排除"
echo ""
