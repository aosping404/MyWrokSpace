#!/bin/bash
# 创建 ROS2 + YOLO26 融合 Conda 环境脚本

set -e

echo "=========================================="
echo "  创建 ROS2 + YOLO26 融合环境"
echo "=========================================="
echo ""

ENV_NAME="ros2_yolo"
YOLO_ENV="yolo"

# 检查 yolo 环境是否存在
if ! conda env list | grep -q "^${YOLO_ENV} "; then
    echo "错误: yolo 环境不存在"
    echo "请先创建 yolo 环境"
    exit 1
fi

echo "步骤 1: 创建新环境 ${ENV_NAME} (Python 3.12，兼容 ROS2 Jazzy)"
echo "注意: yolo 环境使用 Python 3.11，但 ROS2 Jazzy 需要 Python 3.12"
echo ""

# 创建新环境
if conda env list | grep -q "^${ENV_NAME} "; then
    echo "⚠️  环境 ${ENV_NAME} 已存在"
    read -p "是否删除并重新创建? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "删除现有环境..."
        conda env remove -n ${ENV_NAME} -y
    else
        echo "操作已取消"
        exit 1
    fi
fi

echo "创建 Conda 环境: ${ENV_NAME} (Python 3.12)..."
conda create -n ${ENV_NAME} python=3.12 -y

echo ""
echo "步骤 2: 初始化 Conda"
# 初始化 Conda（如果尚未初始化）
if [ -f "$HOME/miniconda3/etc/profile.d/conda.sh" ]; then
    source "$HOME/miniconda3/etc/profile.d/conda.sh"
else
    echo "警告: 找不到 conda.sh，尝试使用 conda run"
fi

echo ""
echo "步骤 3: 安装 PyTorch + CUDA (在 ${ENV_NAME} 环境中)"
echo "安装 PyTorch 2.9.1 + CUDA 12.8..."
echo "注意: 使用 pip 安装 PyTorch，因为 conda 可能不提供 pytorch-cuda=12.8"
conda run -n ${ENV_NAME} pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu128

echo ""
echo "步骤 4: 安装 YOLO26 相关包"
echo "从 yolo 环境复制包列表..."
conda run -n ${ENV_NAME} pip install ultralytics opencv-python

# 可选：安装其他 yolo 环境中的包
echo "安装其他依赖..."
conda run -n ${ENV_NAME} pip install numpy pillow matplotlib

echo ""
echo "步骤 5: 安装 ROS2 Python 支持"
echo "安装 rclpy (ROS2 Python 客户端库)..."
conda run -n ${ENV_NAME} pip install rclpy

echo ""
echo "步骤 5.1: 安装 ROS2 构建系统依赖"
echo "安装 catkin_pkg (ROS2 包解析工具)..."
conda run -n ${ENV_NAME} pip install catkin_pkg empy lark

echo ""
echo "步骤 6: 验证安装"
echo ""

echo "检查 PyTorch:"
conda run -n ${ENV_NAME} python -c "import torch; print(f'  ✓ PyTorch: {torch.__version__}'); print(f'  ✓ CUDA 可用: {torch.cuda.is_available()}'); print(f'  ✓ CUDA 版本: {torch.version.cuda if torch.cuda.is_available() else \"N/A\"}')" || echo "  ✗ PyTorch 检查失败"

echo ""
echo "检查 YOLO26:"
conda run -n ${ENV_NAME} python -c "from ultralytics import YOLO; print('  ✓ YOLO26 (ultralytics) 可用')" || echo "  ✗ YOLO26 检查失败"

echo ""
echo "检查 OpenCV:"
conda run -n ${ENV_NAME} python -c "import cv2; print(f'  ✓ OpenCV: {cv2.__version__}')" || echo "  ✗ OpenCV 检查失败"

echo ""
echo "检查 ROS2 Python:"
conda run -n ${ENV_NAME} python -c "import rclpy; print('  ✓ ROS2 Python (rclpy) 可用')" || echo "  ✗ ROS2 Python 检查失败"

echo ""
echo "检查 ROS2 构建系统:"
conda run -n ${ENV_NAME} python -c "import catkin_pkg; print('  ✓ catkin_pkg 可用')" || echo "  ✗ catkin_pkg 检查失败"

echo ""
echo "=========================================="
echo "  环境创建完成！"
echo "=========================================="
echo ""
echo "下一步："
echo "  1. 更新 robotics.conf 配置文件"
echo "  2. 测试环境切换"
echo ""
echo "使用方法："
echo "  conda activate ${ENV_NAME}"
echo "  source ~/MyWorkSpace/project_env.sh robotics"
echo ""
