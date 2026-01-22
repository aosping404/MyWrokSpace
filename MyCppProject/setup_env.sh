#!/bin/bash
# 环境变量配置脚本

# Google API Key (请替换为你的实际 API Key)
export GOOGLE_API_KEY="YOUR_GOOGLE_API_KEY_HERE"

# GitHub Personal Access Token (请替换为你的实际 token)
export GITHUB_TOKEN="YOUR_GITHUB_TOKEN_HERE"

# GitHub PAT (备选 token，格式: github_pat_...)
export GITHUB_PAT="YOUR_GITHUB_PAT_HERE"

# 将环境变量添加到 ~/.bashrc (可选，持久化配置)
if ! grep -q "GOOGLE_API_KEY" ~/.bashrc 2>/dev/null; then
    echo "" >> ~/.bashrc
    echo "# Google API Key" >> ~/.bashrc
    echo "export GOOGLE_API_KEY=\"YOUR_GOOGLE_API_KEY_HERE\"" >> ~/.bashrc
    echo "" >> ~/.bashrc
    echo "# GitHub Personal Access Token" >> ~/.bashrc
    echo "export GITHUB_TOKEN=\"YOUR_GITHUB_TOKEN_HERE\"" >> ~/.bashrc
    echo "export GITHUB_PAT=\"YOUR_GITHUB_PAT_HERE\"" >> ~/.bashrc
    echo "环境变量已添加到 ~/.bashrc"
fi

echo "✓ 环境变量已设置："
echo "  GOOGLE_API_KEY: ${GOOGLE_API_KEY:0:20}..."
echo "  GITHUB_TOKEN: ${GITHUB_TOKEN:0:10}..."

# 配置 Git 使用 GitHub token（用于 HTTPS 推送）
if [ -n "$GITHUB_TOKEN" ]; then
    echo ""
    echo "配置 Git 使用 GitHub token..."
    
    # 配置 Git 凭据助手（使用 token）
    git config --global credential.helper store
    
    # 为当前目录配置远程仓库（如果存在）
    if [ -d ".git" ]; then
        REPO_NAME=$(basename $(git rev-parse --show-toplevel 2>/dev/null) 2>/dev/null || echo "")
        if [ -n "$REPO_NAME" ]; then
            git remote set-url origin https://${GITHUB_TOKEN}@github.com/aosping404/${REPO_NAME}.git
            echo "✓ 当前仓库 ($REPO_NAME) 已配置为使用 token"
        fi
    fi
    
    echo "✓ Git 凭据助手已配置"
    echo ""
    echo "现在可以使用以下命令推送："
    echo "  git push -u origin main"
    echo ""
    echo "提示：如果仍有权限问题，请确保："
    echo "  1. GitHub token 有正确的权限（repo 权限）"
    echo "  2. 仓库名称和用户名正确"
fi
