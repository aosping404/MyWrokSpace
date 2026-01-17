#!/bin/bash
# Conda 环境设置脚本

# 颜色定义
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  GitHub Actions CI/CD 工具环境设置${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 检查 conda 是否安装
if ! command -v conda &> /dev/null; then
    echo -e "${RED}错误: 未找到 conda 命令${NC}"
    echo -e "${YELLOW}请先安装 Miniconda 或 Anaconda${NC}"
    echo -e "${YELLOW}下载地址: https://docs.conda.io/en/latest/miniconda.html${NC}"
    exit 1
fi

echo -e "${GREEN}✓ 检测到 conda${NC}"
echo ""

# 获取脚本所在目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ENV_FILE="$SCRIPT_DIR/environment.yml"
ENV_NAME="github-actions-ci-tool"

# 检查环境文件是否存在
if [ ! -f "$ENV_FILE" ]; then
    echo -e "${RED}错误: 未找到 environment.yml 文件${NC}"
    exit 1
fi

echo -e "${BLUE}正在创建 conda 环境: ${ENV_NAME}${NC}"
echo ""

# 创建 conda 环境
conda env create -f "$ENV_FILE"

if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✓ 环境创建成功!${NC}"
    echo ""
    echo -e "${YELLOW}下一步:${NC}"
    echo -e "  1. 激活环境: ${GREEN}conda activate ${ENV_NAME}${NC}"
    echo -e "  2. 运行工具: ${GREEN}python create_github_actions_ci.py${NC}"
    echo ""
    echo -e "${YELLOW}其他常用命令:${NC}"
    echo -e "  - 停用环境: ${GREEN}conda deactivate${NC}"
    echo -e "  - 删除环境: ${GREEN}conda env remove -n ${ENV_NAME}${NC}"
    echo -e "  - 列出所有环境: ${GREEN}conda env list${NC}"
    echo ""
else
    echo ""
    echo -e "${RED}✗ 环境创建失败${NC}"
    echo -e "${YELLOW}提示: 如果环境已存在,可以使用以下命令更新:${NC}"
    echo -e "  ${GREEN}conda env update -f $ENV_FILE --prune${NC}"
    echo ""
    exit 1
fi
