#!/bin/bash
# Git 仓库初始化脚本

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

# 检查 README.md 是否存在
if [ ! -f "README.md" ]; then
    echo "创建 README.md..."
    echo "# MyWorkSpace" > README.md
fi

# 初始化 Git 仓库
if [ ! -d ".git" ]; then
    echo "初始化 Git 仓库..."
    git init
    echo "✓ Git 仓库初始化完成"
fi

# 添加所有文件
echo "添加文件到 Git..."
git add .

# 检查是否有远程仓库
if git remote | grep -q "^origin$"; then
    echo "⚠️  远程仓库 'origin' 已存在"
    read -p "是否更新远程仓库地址？(y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        git remote set-url origin https://github.com/aosping404/MyWrokSpace.git
        echo "✓ 远程仓库地址已更新"
    fi
else
    echo "添加远程仓库..."
    git remote add origin https://github.com/aosping404/MyWrokSpace.git
    echo "✓ 远程仓库已添加"
fi

# 设置主分支
echo "设置主分支为 main..."
git branch -M main

# 首次提交
if [ -z "$(git log --oneline 2>/dev/null)" ]; then
    echo "创建首次提交..."
    git commit -m "first commit: Add MyWorkSpace projects and documentation"
    echo "✓ 首次提交完成"
else
    echo "⚠️  已有提交记录，跳过首次提交"
fi

echo ""
echo "=========================================="
echo "  初始化完成！"
echo "=========================================="
echo ""
echo "下一步："
echo "  1. 检查文件: git status"
echo "  2. 提交更改: git commit -m 'your message'"
echo "  3. 推送到远程: git push -u origin main"
echo ""
