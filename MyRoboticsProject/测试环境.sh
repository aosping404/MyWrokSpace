#!/bin/bash
# 测试 Robotics 环境脚本

echo "=========================================="
echo "  测试 Robotics 环境"
echo "=========================================="
echo ""

# 切换到 robotics 环境
cd ~/MyWorkSpace
source project_env.sh robotics

echo ""
echo "=== 环境验证 ==="
echo ""

echo "1. Conda 环境:"
echo "   当前环境: $CONDA_DEFAULT_ENV"
echo "   Conda 路径: $CONDA_PREFIX"

echo ""
echo "2. ROS2 环境:"
echo "   分布版本: $ROS_DISTRO"
echo "   ROS 版本: $ROS_VERSION"

echo ""
echo "3. MoveIt2 环境:"
if [[ "$PATH" == *"ws_moveit2"* ]]; then
    echo "   已激活 (在PATH中检测到)"
else
    echo "   未激活"
fi

echo ""
echo "4. CUDA 环境:"
echo "   CUDA 路径: $CUDA_HOME"

echo ""
echo "5. Python 版本:"
# 使用 conda 环境中的 python（优先）或 python3
PYTHON_CMD=$(which python 2>/dev/null || which python3)
if [ -z "$PYTHON_CMD" ]; then
    echo "   ✗ Python 未找到"
else
    $PYTHON_CMD --version
    echo "   Python 路径: $PYTHON_CMD"
fi

echo ""
echo "6. PyTorch 和 CUDA:"
$PYTHON_CMD -c "import torch; print(f'   ✓ PyTorch: {torch.__version__}'); print(f'   ✓ CUDA 可用: {torch.cuda.is_available()}'); print(f'   ✓ CUDA 版本: {torch.version.cuda if torch.cuda.is_available() else \"N/A\"}')" 2>/dev/null || echo "   ✗ PyTorch 不可用"

echo ""
echo "7. YOLO26:"
$PYTHON_CMD -c "from ultralytics import YOLO; print('   ✓ YOLO26 (ultralytics) 可用')" 2>/dev/null || echo "   ✗ YOLO26 不可用"

echo ""
echo "8. OpenCV:"
$PYTHON_CMD -c "import cv2; print(f'   ✓ OpenCV: {cv2.__version__}')" 2>/dev/null || echo "   ✗ OpenCV 不可用"

echo ""
echo "9. ROS2 Python:"
$PYTHON_CMD -c "import rclpy; print('   ✓ ROS2 Python (rclpy) 可用')" 2>/dev/null || echo "   ✗ ROS2 Python 不可用"

echo ""
echo "=========================================="
echo "  环境测试完成！"
echo "=========================================="
echo ""
echo "如果所有检查都通过 ✓，环境配置成功！"
echo ""
