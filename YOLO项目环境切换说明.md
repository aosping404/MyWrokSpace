# YOLOv8 项目环境切换说明

## 快速开始

YOLOv8项目已预配置环境隔离，可以直接使用：

```bash
# 切换到YOLO项目环境
source project_env.sh yolo

# 验证环境
python -c "import torch; print(f'PyTorch: {torch.__version__}, CUDA可用: {torch.cuda.is_available()}')"
```

## 环境配置

YOLO项目配置文件位于：`.project_env_configs/yolo.conf`

```bash
# YOLOv8项目配置
CONDA_ENV="yolo"        # 使用专门的yolo Conda环境（包含PyTorch、torchvision、torchaudio等）
CUDA_VERSION="12.8"     # YOLOv8需要CUDA支持
# ROS2和MoveIt2均不激活，避免依赖冲突
```

## 环境隔离特性

### ✅ 自动隔离ROS2环境

切换YOLO环境时，脚本会自动：
- 清理ROS2相关环境变量（ROS_DISTRO, ROS_VERSION等）
- 移除ROS2路径（/opt/ros/jazzy/*）
- 清理MoveIt2工作空间路径
- 清理PYTHONPATH中的ROS2包路径

### ✅ Conda环境切换

- 从任何其他Conda环境切换到yolo环境
- 自动停用之前的环境
- 激活yolo环境（包含PyTorch、CUDA库等）

### ✅ CUDA支持

- 自动配置CUDA 12.8路径
- 设置CUDA_HOME和CUDA_PATH环境变量
- 配置LD_LIBRARY_PATH包含CUDA库路径

## 使用场景

### 场景1: 从ROS2项目切换到YOLO项目

```bash
# 在ROS2项目终端中
source project_env.sh ros2_project  # 当前使用ROS2环境

# 切换到YOLO项目（自动清理ROS2环境）
source project_env.sh yolo
# ✅ ROS2环境已清理
# ✅ YOLO环境已激活
```

### 场景2: 在独立终端中使用

```bash
# 终端1 - YOLO项目
source project_env.sh yolo
python train.py

# 终端2 - ROS2项目
source project_env.sh ros2_project
ros2 run my_package my_node
```

### 场景3: 验证环境隔离

```bash
# 切换到YOLO环境
source project_env.sh yolo

# 查看当前环境状态
source project_env.sh --show

# 应该显示：
# - Conda环境: yolo
# - CUDA环境: 已激活
# - ROS2环境: 未激活
# - MoveIt2环境: 未激活
```

## 环境验证

### 检查PyTorch和CUDA

```bash
python -c "
import torch
print(f'PyTorch版本: {torch.__version__}')
print(f'CUDA可用: {torch.cuda.is_available()}')
if torch.cuda.is_available():
    print(f'CUDA版本: {torch.version.cuda}')
    print(f'GPU数量: {torch.cuda.device_count()}')
    print(f'GPU名称: {torch.cuda.get_device_name(0)}')
"
```

### 检查已安装的包

```bash
pip list | grep -E "torch|nvidia|cuda"
```

### 检查环境变量

```bash
echo "CONDA环境: $CONDA_DEFAULT_ENV"
echo "CUDA路径: $CUDA_HOME"
echo "ROS2分布: ${ROS_DISTRO:-未设置}"  # 应该是"未设置"
```

## 常见问题

### Q1: YOLO环境与ROS2环境冲突怎么办？

**A**: 脚本已经自动处理了。切换环境时会自动清理冲突的环境变量和路径。如果仍有问题，确保：
1. 使用最新的脚本版本
2. 在不同的终端中使用不同的环境
3. 使用 `source project_env.sh --show` 检查当前状态

### Q2: 如何确认ROS2环境已被清理？

```bash
source project_env.sh yolo
source project_env.sh --show
# 查看ROS2环境应该显示"未激活"
```

### Q3: PyTorch无法检测到CUDA？

```bash
# 检查CUDA环境变量
echo $CUDA_HOME
echo $LD_LIBRARY_PATH | grep cuda

# 验证nvidia-smi
nvidia-smi

# 重新切换环境
source project_env.sh yolo
```

### Q4: 如何在YOLO环境中安装新包？

```bash
source project_env.sh yolo
pip install package_name
```

**注意**: 确保在YOLO环境激活状态下安装，避免安装到错误的环境。

## 配置文件位置

- 配置文件: `.project_env_configs/yolo.conf`
- 脚本位置: `project_env.sh`
- Conda环境: `~/miniconda3/envs/yolo/`

## 相关命令

```bash
# 切换到YOLO环境
source project_env.sh yolo

# 列出所有项目配置
source project_env.sh --list

# 显示当前环境状态
source project_env.sh --show

# 显示帮助
source project_env.sh --help
```

## 最佳实践

1. **独立终端使用**: 如果同时需要ROS2和YOLO，在不同的终端窗口中使用
2. **切换前验证**: 切换环境后使用 `--show` 验证环境状态
3. **包管理**: 在对应环境激活状态下安装包
4. **环境检查**: 运行关键代码前先验证环境配置

---

**提示**: 如需修改YOLO项目配置，编辑 `.project_env_configs/yolo.conf` 文件。
