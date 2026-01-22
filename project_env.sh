#!/bin/bash

###############################################################################
# 项目环境隔离切换脚本
# 用途: 为不同项目切换和隔离开发环境，避免环境冲突
# 使用方法: source project_env.sh [project_name]
###############################################################################

# 脚本目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_DIR="$SCRIPT_DIR/.project_env_configs"
CONFIG_FILE="$CONFIG_DIR/default.conf"

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 环境变量备份（用于恢复）
declare -A ENV_BACKUP

###############################################################################
# 工具函数
###############################################################################

# 打印信息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 备份环境变量
backup_env_var() {
    local var_name=$1
    ENV_BACKUP["$var_name"]="${!var_name}"
}

# 恢复环境变量
restore_env_var() {
    local var_name=$1
    if [[ -n "${ENV_BACKUP[$var_name]}" ]]; then
        export "$var_name"="${ENV_BACKUP[$var_name]}"
    else
        unset "$var_name"
    fi
}

# 清理PATH中的重复项
clean_path() {
    local path_var=$1
    local cleaned_path=$(echo "${!path_var}" | tr ':' '\n' | awk '!seen[$0]++' | tr '\n' ':' | sed 's/:$//')
    export "$path_var"="$cleaned_path"
}

###############################################################################
# 环境清理函数
###############################################################################

# 清理Conda环境
cleanup_conda() {
    # 如果当前在Conda环境中（非base），需要多次deactivate直到回到base
    while [[ -n "$CONDA_DEFAULT_ENV" && "$CONDA_DEFAULT_ENV" != "base" && "$CONDA_SHLVL" -gt 1 ]]; do
        print_info "停用Conda环境: $CONDA_DEFAULT_ENV (层级: $CONDA_SHLVL)"
        conda deactivate 2>/dev/null || break
    done
    
    # 如果仍在非base环境，再次尝试deactivate
    if [[ -n "$CONDA_DEFAULT_ENV" && "$CONDA_DEFAULT_ENV" != "base" ]]; then
        print_info "停用Conda环境: $CONDA_DEFAULT_ENV"
        conda deactivate 2>/dev/null || true
    fi
    
    # 备份Conda相关环境变量
    backup_env_var "CONDA_DEFAULT_ENV"
    backup_env_var "CONDA_PREFIX"
    backup_env_var "CONDA_SHLVL"
    backup_env_var "CONDA_EXE"
    backup_env_var "CONDA_PYTHON_EXE"
    backup_env_var "CONDA_PROMPT_MODIFIER"
    
    # 从PATH中移除所有Conda环境路径（保留base环境的miniconda3/bin）
    # 移除特定环境的bin目录
    if [[ -n "$CONDA_PREFIX" && "$CONDA_PREFIX" != "$HOME/miniconda3" ]]; then
        export PATH=$(echo "$PATH" | sed "s|$CONDA_PREFIX/bin:||g" | sed "s|$CONDA_PREFIX/condabin:||g")
    fi
    
    # 如果不在base环境，确保清理环境特定的Python路径
    if [[ -n "$CONDA_DEFAULT_ENV" && "$CONDA_DEFAULT_ENV" != "base" ]]; then
        # 从PYTHONPATH中移除Conda环境路径
        if [[ -n "$PYTHONPATH" ]]; then
            export PYTHONPATH=$(echo "$PYTHONPATH" | sed "s|$CONDA_PREFIX/lib/python[^:]*:||g")
        fi
    fi
}

# 清理ROS2环境
cleanup_ros2() {
    backup_env_var "ROS_DISTRO"
    backup_env_var "ROS_VERSION"
    backup_env_var "ROS_PYTHON_VERSION"
    backup_env_var "AMENT_PREFIX_PATH"
    backup_env_var "CMAKE_PREFIX_PATH"
    backup_env_var "PYTHONPATH"
    backup_env_var "LD_LIBRARY_PATH"
    
    # 从PATH中移除ROS2路径
    export PATH=$(echo "$PATH" | sed "s|/opt/ros/[^:]*/bin:||g" | sed "s|/opt/ros/[^:]*/opt/[^:]*/bin:||g")
    
    # 从LD_LIBRARY_PATH中移除ROS2库路径
    if [[ -n "$LD_LIBRARY_PATH" ]]; then
        export LD_LIBRARY_PATH=$(echo "$LD_LIBRARY_PATH" | sed "s|/opt/ros/[^:]*[^:]*:||g")
    fi
    
    # 从PYTHONPATH中移除ROS2路径
    if [[ -n "$PYTHONPATH" ]]; then
        export PYTHONPATH=$(echo "$PYTHONPATH" | sed "s|/opt/ros/[^:]*[^:]*:||g")
    fi
    
    unset ROS_DISTRO
    unset ROS_VERSION
    unset ROS_PYTHON_VERSION
    unset AMENT_PREFIX_PATH
}

# 清理MoveIt2环境
cleanup_moveit2() {
    backup_env_var "CMAKE_PREFIX_PATH"
    backup_env_var "LD_LIBRARY_PATH"
    backup_env_var "PYTHONPATH"
    
    # 从PATH中移除MoveIt2路径
    export PATH=$(echo "$PATH" | sed "s|$HOME/ws_moveit2/install[^:]*:||g")
    
    # 从库路径中移除MoveIt2路径
    if [[ -n "$LD_LIBRARY_PATH" ]]; then
        export LD_LIBRARY_PATH=$(echo "$LD_LIBRARY_PATH" | sed "s|$HOME/ws_moveit2/install[^:]*:||g")
    fi
    
    # 从PYTHONPATH中移除MoveIt2路径
    if [[ -n "$PYTHONPATH" ]]; then
        export PYTHONPATH=$(echo "$PYTHONPATH" | sed "s|$HOME/ws_moveit2/install[^:]*:||g")
    fi
}

# 清理HALCON环境
cleanup_halcon() {
    backup_env_var "HALCONROOT"
    backup_env_var "HALCONARCH"
    backup_env_var "HALCONEXAMPLES"
    backup_env_var "HALCON_DL_ROOT"
    
    # 从PATH中移除HALCON路径
    if [[ -n "$HALCONROOT" ]]; then
        export PATH=$(echo "$PATH" | sed "s|$HALCONROOT/bin/[^:]*:||g")
        
        # 从LD_LIBRARY_PATH中移除HALCON库路径
        if [[ -n "$LD_LIBRARY_PATH" ]]; then
            export LD_LIBRARY_PATH=$(echo "$LD_LIBRARY_PATH" | sed "s|$HALCONROOT/lib/[^:]*:||g" | sed "s|$HALCONROOT/lib/[^:]*/thirdparty:||g")
        fi
    fi
    
    unset HALCONROOT
    unset HALCONARCH
    unset HALCONEXAMPLES
    unset HALCON_DL_ROOT
}

# 清理CUDA环境
cleanup_cuda() {
    backup_env_var "LD_LIBRARY_PATH"
    backup_env_var "CUDA_HOME"
    backup_env_var "CUDA_PATH"
    
    # 从PATH中移除CUDA路径
    export PATH=$(echo "$PATH" | sed "s|/usr/local/cuda[^:]*/bin:||g")
    
    # 从LD_LIBRARY_PATH中移除CUDA库路径
    if [[ -n "$LD_LIBRARY_PATH" ]]; then
        export LD_LIBRARY_PATH=$(echo "$LD_LIBRARY_PATH" | sed "s|/usr/local/cuda[^:]*/lib64:||g")
    fi
    
    unset CUDA_HOME
    unset CUDA_PATH
}

# 清理Node.js环境
cleanup_nodejs() {
    backup_env_var "NVM_DIR"
    
    # 从PATH中移除Node.js路径
    export PATH=$(echo "$PATH" | sed "s|$HOME/.nvm/versions/node/[^:]*/bin:||g")
    export PATH=$(echo "$PATH" | sed "s|$HOME/.local/share/pnpm:||g")
    
    # 卸载NVM函数
    unset -f nvm &>/dev/null || true
}

###############################################################################
# 环境激活函数
###############################################################################

# 激活Conda环境
activate_conda() {
    local env_name=$1
    if [[ -z "$env_name" ]]; then
        return 0
    fi
    
    # 初始化Conda（如果尚未初始化或已被清理）
    if [[ -z "$CONDA_EXE" ]] || ! command -v conda &> /dev/null; then
        if [[ -f "$HOME/miniconda3/etc/profile.d/conda.sh" ]]; then
            source "$HOME/miniconda3/etc/profile.d/conda.sh"
        else
            print_warning "Conda初始化脚本未找到，尝试使用系统PATH中的conda"
        fi
    fi
    
    # 如果当前已激活目标环境，直接返回
    if [[ "$CONDA_DEFAULT_ENV" == "$env_name" ]]; then
        print_info "Conda环境已激活: $env_name"
        return 0
    fi
    
    # 如果当前在其他环境，先确保回到base（如果需要）
    if [[ -n "$CONDA_DEFAULT_ENV" && "$CONDA_DEFAULT_ENV" != "base" && "$env_name" != "$CONDA_DEFAULT_ENV" ]]; then
        # 已经在cleanup_conda中处理了，这里只需要确保切换
        print_info "从环境 $CONDA_DEFAULT_ENV 切换到 $env_name"
    fi
    
    if [[ "$env_name" == "base" ]]; then
        print_info "使用Conda base环境"
        # base环境通常已在PATH中，但确保conda命令可用
        if [[ -z "$CONDA_DEFAULT_ENV" ]]; then
            # 如果不在任何环境，可以尝试激活base（如果需要）
            # 通常不需要，因为base环境已经在PATH中
            return 0
        fi
        return 0
    fi
    
    # 检查环境是否存在
    local env_exists=false
    if command -v conda &> /dev/null; then
        if conda env list 2>/dev/null | grep -qE "^${env_name}[[:space:]]|^[[:space:]]*${env_name}[[:space:]]"; then
            env_exists=true
        fi
    else
        # 如果conda命令不可用，直接检查目录
        if [[ -d "$HOME/miniconda3/envs/$env_name" ]]; then
            env_exists=true
        fi
    fi
    
    if [[ "$env_exists" == true ]]; then
        print_info "激活Conda环境: $env_name"
        # 确保Conda已初始化
        if [[ -z "$CONDA_EXE" ]] && [[ -f "$HOME/miniconda3/etc/profile.d/conda.sh" ]]; then
            source "$HOME/miniconda3/etc/profile.d/conda.sh"
        fi
        conda activate "$env_name" 2>/dev/null || {
            print_error "无法激活Conda环境: $env_name"
            return 1
        }
    else
        print_warning "Conda环境不存在: $env_name"
        print_info "可用的Conda环境："
        conda env list 2>/dev/null | grep -v "^#" || echo "  无法列出环境列表"
        return 1
    fi
}

# 激活ROS2环境
activate_ros2() {
    local ros_distro=${1:-jazzy}
    local ros_path="/opt/ros/$ros_distro"
    
    if [[ ! -d "$ros_path" ]]; then
        print_warning "ROS2分布不存在: $ros_distro ($ros_path)"
        return 1
    fi
    
    print_info "激活ROS2环境: $ros_distro"
    source "$ros_path/setup.bash"
}

# 激活MoveIt2环境
activate_moveit2() {
    local ws_path=${1:-"$HOME/ws_moveit2"}
    local install_path="$ws_path/install"
    
    # 检查标准位置
    if [[ ! -d "$install_path" ]]; then
        # 检查非标准位置（install 目录在 src/ 下）
        install_path="$ws_path/src/install"
        if [[ ! -d "$install_path" ]]; then
            print_warning "MoveIt2工作空间不存在: $ws_path/install 或 $ws_path/src/install"
            print_info "请先编译MoveIt2工作空间"
            return 1
        else
            print_info "检测到非标准位置: $install_path"
        fi
    fi
    
    print_info "激活MoveIt2工作空间: $ws_path (install: $install_path)"
    source "$install_path/setup.bash"
}

# 激活HALCON环境
activate_halcon() {
    local halcon_root=${1:-/opt/halcon}
    
    if [[ ! -d "$halcon_root" ]]; then
        print_warning "HALCON路径不存在: $halcon_root"
        return 1
    fi
    
    print_info "激活HALCON环境: $halcon_root"
    export HALCONROOT="$halcon_root"
    export HALCONARCH=x64-linux
    export HALCONEXAMPLES=$HALCONROOT/examples
    export PATH=$HALCONROOT/bin/$HALCONARCH:$PATH
    export LD_LIBRARY_PATH=$HALCONROOT/lib/$HALCONARCH:$LD_LIBRARY_PATH
    export HALCON_DL_ROOT=$HALCONROOT/dl
    export LD_LIBRARY_PATH=$HALCONROOT/lib/$HALCONARCH/thirdparty:$LD_LIBRARY_PATH
}

# 激活CUDA环境
activate_cuda() {
    local cuda_version=${1:-12.8}
    local cuda_path="/usr/local/cuda-$cuda_version"
    
    if [[ ! -d "$cuda_path" ]]; then
        print_warning "CUDA路径不存在: $cuda_path"
        return 1
    fi
    
    print_info "激活CUDA环境: $cuda_version"
    export PATH=$cuda_path/bin:$PATH
    export LD_LIBRARY_PATH=$cuda_path/lib64:$LD_LIBRARY_PATH
    export CUDA_HOME=$cuda_path
    export CUDA_PATH=$cuda_path
}

# 激活Node.js环境
activate_nodejs() {
    local node_version=${1:-v20.19.5}
    local node_path="$HOME/.nvm/versions/node/$node_version"
    
    if [[ ! -d "$node_path" ]]; then
        print_warning "Node.js版本不存在: $node_version ($node_path)"
        return 1
    fi
    
    print_info "激活Node.js环境: $node_version"
    export PATH=$node_path/bin:$PATH
    
    # 激活pnpm
    if [[ -d "$HOME/.local/share/pnpm" ]]; then
        export PNPM_HOME="$HOME/.local/share/pnpm"
        export PATH=$PNPM_HOME:$PATH
    fi
}

###############################################################################
# 配置管理函数
###############################################################################

# 创建配置目录
ensure_config_dir() {
    if [[ ! -d "$CONFIG_DIR" ]]; then
        mkdir -p "$CONFIG_DIR"
    fi
}

# 加载项目配置
load_project_config() {
    local project_name=$1
    local config_file="$CONFIG_DIR/${project_name}.conf"
    
    if [[ ! -f "$config_file" ]]; then
        print_warning "项目配置不存在: $project_name"
        print_info "创建默认配置..."
        create_default_config "$project_name"
        config_file="$CONFIG_DIR/${project_name}.conf"
    fi
    
    source "$config_file"
}

# 创建默认配置
create_default_config() {
    local project_name=${1:-default}
    local config_file="$CONFIG_DIR/${project_name}.conf"
    
    cat > "$config_file" <<EOF
# 项目环境配置: $project_name
# 生成时间: $(date '+%Y-%m-%d %H:%M:%S')

# Conda环境 (留空表示不使用Conda，或使用"base")
CONDA_ENV="base"

# ROS2配置 (留空表示不使用ROS2)
# ROS_DISTRO="jazzy"

# MoveIt2配置 (留空表示不使用MoveIt2)
# MOVEIT2_WS="\$HOME/ws_moveit2"

# HALCON配置 (留空表示不使用HALCON)
# HALCON_ROOT="/opt/halcon"

# CUDA配置 (留空表示不使用CUDA)
# CUDA_VERSION="12.8"

# Node.js配置 (留空表示不使用Node.js)
# NODE_VERSION="v20.19.5"
EOF
    
    print_success "已创建默认配置: $config_file"
}

# 显示当前环境状态
show_current_env() {
    echo ""
    echo "=========================================="
    echo "  当前环境状态"
    echo "=========================================="
    echo ""
    
    echo "Conda环境:"
    if [[ -n "$CONDA_DEFAULT_ENV" ]]; then
        echo "  - 当前环境: $CONDA_DEFAULT_ENV"
        echo "  - Conda路径: $CONDA_PREFIX"
    else
        echo "  - 未激活"
    fi
    
    echo ""
    echo "ROS2环境:"
    if [[ -n "$ROS_DISTRO" ]]; then
        echo "  - 分布版本: $ROS_DISTRO"
        echo "  - ROS版本: $ROS_VERSION"
    else
        echo "  - 未激活"
    fi
    
    echo ""
    echo "MoveIt2环境:"
    if [[ "$PATH" == *"ws_moveit2"* ]]; then
        echo "  - 已激活 (在PATH中检测到)"
    else
        echo "  - 未激活"
    fi
    
    echo ""
    echo "HALCON环境:"
    if [[ -n "$HALCONROOT" ]]; then
        echo "  - HALCON根目录: $HALCONROOT"
        echo "  - 架构: $HALCONARCH"
    else
        echo "  - 未激活"
    fi
    
    echo ""
    echo "CUDA环境:"
    if [[ -n "$CUDA_HOME" ]]; then
        echo "  - CUDA路径: $CUDA_HOME"
    elif [[ "$PATH" == *"cuda"* ]]; then
        echo "  - 已激活 (在PATH中检测到)"
    else
        echo "  - 未激活"
    fi
    
    echo ""
    echo "Node.js环境:"
    if command -v node &> /dev/null; then
        echo "  - Node.js版本: $(node --version 2>/dev/null)"
        echo "  - npm版本: $(npm --version 2>/dev/null 2>/dev/null || echo 'N/A')"
    else
        echo "  - 未激活"
    fi
    
    echo ""
    echo "=========================================="
}

###############################################################################
# 主函数
###############################################################################

# 切换项目环境
switch_project_env() {
    local project_name=${1:-default}
    
    print_info "切换到项目环境: $project_name"
    echo ""
    
    # 清理现有环境
    print_info "清理现有环境..."
    cleanup_conda
    cleanup_ros2
    cleanup_moveit2
    cleanup_halcon
    cleanup_cuda
    cleanup_nodejs
    
    # 清理PATH和LD_LIBRARY_PATH中的重复项
    clean_path "PATH"
    clean_path "LD_LIBRARY_PATH"
    
    echo ""
    
    # 加载项目配置
    ensure_config_dir
    load_project_config "$project_name"
    
    # 激活配置的环境
    local activated=0
    
    # 激活Conda环境
    if [[ -n "$CONDA_ENV" ]]; then
        activate_conda "$CONDA_ENV" && ((activated++))
    fi
    
    # 激活ROS2环境
    if [[ -n "$ROS_DISTRO" ]]; then
        activate_ros2 "$ROS_DISTRO" && ((activated++))
    fi
    
    # 激活MoveIt2环境
    if [[ -n "$MOVEIT2_WS" ]]; then
        activate_moveit2 "$MOVEIT2_WS" && ((activated++))
    fi
    
    # 激活HALCON环境
    if [[ -n "$HALCON_ROOT" ]]; then
        activate_halcon "$HALCON_ROOT" && ((activated++))
    fi
    
    # 激活CUDA环境
    if [[ -n "$CUDA_VERSION" ]]; then
        activate_cuda "$CUDA_VERSION" && ((activated++))
    fi
    
    # 激活Node.js环境
    if [[ -n "$NODE_VERSION" ]]; then
        activate_nodejs "$NODE_VERSION" && ((activated++))
    fi
    
    # 再次清理PATH中的重复项
    clean_path "PATH"
    clean_path "LD_LIBRARY_PATH"
    
    echo ""
    print_success "环境切换完成 ($activated 个组件已激活)"
    show_current_env
}

# 列出所有可用的项目配置
list_projects() {
    ensure_config_dir
    echo ""
    echo "可用的项目配置:"
    echo ""
    
    if [[ -d "$CONFIG_DIR" && -n "$(ls -A "$CONFIG_DIR"/*.conf 2>/dev/null)" ]]; then
        for config_file in "$CONFIG_DIR"/*.conf; do
            local project_name=$(basename "$config_file" .conf)
            echo "  - $project_name"
        done
    else
        echo "  没有找到项目配置"
        echo "  运行 'source project_env.sh' 创建默认配置"
    fi
    echo ""
}

# 交互式选择项目
interactive_select() {
    ensure_config_dir
    
    # 获取所有项目配置
    local projects=()
    if [[ -d "$CONFIG_DIR" ]]; then
        for config_file in "$CONFIG_DIR"/*.conf; do
            if [[ -f "$config_file" ]]; then
                projects+=("$(basename "$config_file" .conf)")
            fi
        done
    fi
    
    # 如果没有配置，创建默认配置
    if [[ ${#projects[@]} -eq 0 ]]; then
        create_default_config "default"
        projects=("default")
    fi
    
    # 显示菜单
    echo ""
    echo "=========================================="
    echo "  项目环境选择器"
    echo "=========================================="
    echo ""
    echo "请选择要切换到的项目环境:"
    echo ""
    
    local i=1
    for project in "${projects[@]}"; do
        echo "  $i) $project"
        ((i++))
    done
    echo "  $i) 显示当前环境状态"
    ((i++))
    echo "  $i) 退出"
    echo ""
    read -p "请输入选项 [1-$i]: " choice
    
    # 处理选择
    if [[ "$choice" =~ ^[0-9]+$ ]]; then
        if [[ "$choice" -ge 1 && "$choice" -le ${#projects[@]} ]]; then
            local selected_project="${projects[$((choice-1))]}"
            switch_project_env "$selected_project"
        elif [[ "$choice" -eq $((${#projects[@]}+1)) ]]; then
            show_current_env
        elif [[ "$choice" -eq $((${#projects[@]}+2)) ]]; then
            print_info "退出"
            return 0
        else
            print_error "无效的选项"
            return 1
        fi
    else
        print_error "无效的输入"
        return 1
    fi
}

###############################################################################
# 脚本入口
###############################################################################

# 检查脚本是否被source执行（不是直接执行）
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    print_error "此脚本必须使用 'source' 命令执行"
    print_info "使用方法: source $0 [project_name]"
    print_info "示例: source $0 default"
    print_info "      source $0 ros2_project"
    exit 1
fi

# 处理命令行参数
if [[ $# -eq 0 ]]; then
    # 没有参数，显示交互式菜单
    interactive_select
elif [[ "$1" == "--list" || "$1" == "-l" ]]; then
    # 列出所有项目配置
    list_projects
elif [[ "$1" == "--show" || "$1" == "-s" ]]; then
    # 显示当前环境状态
    show_current_env
elif [[ "$1" == "--help" || "$1" == "-h" ]]; then
    # 显示帮助信息
    echo ""
    echo "=========================================="
    echo "  项目环境隔离切换脚本 - 使用说明"
    echo "=========================================="
    echo ""
    echo "用途: 为不同项目切换和隔离开发环境，避免环境冲突"
    echo ""
    echo "使用方法:"
    echo "  source $0 [选项] [项目名称]"
    echo ""
    echo "选项:"
    echo "  -l, --list      列出所有可用的项目配置"
    echo "  -s, --show      显示当前环境状态"
    echo "  -h, --help      显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  source $0                    # 交互式选择项目"
    echo "  source $0 default            # 切换到default项目环境"
    echo "  source $0 ros2_project       # 切换到ros2_project项目环境"
    echo "  source $0 --list             # 列出所有项目配置"
    echo "  source $0 --show             # 显示当前环境状态"
    echo ""
    echo "配置文件位置:"
    echo "  $CONFIG_DIR/"
    echo ""
    echo "可用环境组件:"
    echo "  - Conda环境 (Python包管理)"
    echo "  - ROS2环境 (机器人操作系统)"
    echo "  - MoveIt2环境 (运动规划)"
    echo "  - HALCON环境 (机器视觉)"
    echo "  - CUDA环境 (GPU计算)"
    echo "  - Node.js环境 (JavaScript运行时)"
    echo ""
    echo "提示:"
    echo "  - 每次切换环境会自动清理之前的环境配置"
    echo "  - 配置文件使用 .conf 格式，可以手动编辑"
    echo "  - 项目配置保存在: $CONFIG_DIR/"
    echo ""
else
    # 切换到指定的项目环境
    switch_project_env "$1"
fi

# 设置别名方便使用
alias penv='source "$SCRIPT_DIR/project_env.sh"'
alias penv-list='source "$SCRIPT_DIR/project_env.sh" --list'
alias penv-show='source "$SCRIPT_DIR/project_env.sh" --show'