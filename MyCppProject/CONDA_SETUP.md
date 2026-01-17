# Conda 环境设置指南

本指南介绍如何使用 Conda 创建一个隔离的 Python 环境来运行 GitHub Actions CI/CD 配置生成工具。

## 为什么使用 Conda?

- ✅ **隔离环境**: 不会影响系统 Python 环境
- ✅ **依赖管理**: 自动管理 Python 和系统依赖
- ✅ **跨平台**: 支持 Windows、macOS、Linux
- ✅ **易于管理**: 简单的创建、激活、删除命令

## 快速开始

### 1. 安装 Conda

如果还没有安装 Conda,请先安装:

- **Miniconda** (推荐,体积小): https://docs.conda.io/en/latest/miniconda.html
- **Anaconda** (完整版): https://www.anaconda.com/products/distribution

### 2. 创建环境

#### 方法 A: 使用自动脚本 (推荐)

```bash
# 给脚本添加执行权限
chmod +x conda_setup.sh

# 运行脚本创建环境
./conda_setup.sh
```

#### 方法 B: 手动创建

```bash
# 使用 environment.yml 创建环境
conda env create -f environment.yml

# 激活环境
conda activate github-actions-ci-tool
```

### 3. 使用环境

```bash
# 激活环境 (每次使用前都需要激活)
conda activate github-actions-ci-tool

# 运行工具
python create_github_actions_ci.py

# 使用完毕后退出环境 (可选)
conda deactivate
```

## 常用命令

### 环境管理

```bash
# 列出所有 conda 环境
conda env list
# 或
conda info --envs

# 激活环境
conda activate github-actions-ci-tool

# 停用当前环境
conda deactivate

# 查看当前环境信息
conda info

# 查看当前环境中的包
conda list
```

### 更新和删除

```bash
# 更新环境 (如果修改了 environment.yml)
conda env update -f environment.yml --prune

# 删除环境
conda env remove -n github-actions-ci-tool

# 导出当前环境配置
conda env export > environment_export.yml
```

### 包管理

```bash
# 安装包
conda install package_name

# 使用 pip 安装包 (如果在 conda 环境中)
pip install package_name

# 卸载包
conda remove package_name

# 更新包
conda update package_name
```

## 环境文件说明

### environment.yml

这是 Conda 环境配置文件,定义了:

- **name**: 环境名称 `github-actions-ci-tool`
- **channels**: 包源 (conda-forge, defaults)
- **dependencies**: 依赖包列表
  - `python=3.10`: Python 版本
  - `pyqt=6`: PyQt 6 (通过 conda-forge)
  - `pip`: pip 工具
  - `pip` 下的包: 通过 pip 安装的包

## 故障排除

### 问题 1: conda 命令未找到

**解决方案**:

```bash
# Linux/macOS: 将 conda 添加到 PATH
export PATH="$HOME/miniconda3/bin:$PATH"

# 或初始化 conda (推荐,永久生效)
conda init bash  # 对于 bash
conda init zsh   # 对于 zsh

# 然后重新打开终端
```

### 问题 2: 环境已存在

**解决方案**:

```bash
# 删除旧环境后重新创建
conda env remove -n github-actions-ci-tool
conda env create -f environment.yml

# 或直接更新现有环境
conda env update -f environment.yml --prune
```

### 问题 3: PyQt 安装失败

**解决方案**:

```bash
# 尝试使用 conda-forge 通道
conda install -c conda-forge pyqt

# 或使用 pip 安装
pip install PyQt6
```

### 问题 4: 无法激活环境

**解决方案**:

```bash
# 检查环境是否存在
conda env list

# 如果环境不存在,重新创建
conda env create -f environment.yml

# 确认 conda 已正确初始化
conda init
```

## 环境配置详情

当前环境包含以下包:

| 包名 | 版本 | 用途 |
|------|------|------|
| Python | 3.10 | Python 解释器 |
| PyQt | 6.x | Qt GUI 框架 |
| pip | latest | Python 包管理器 |

## 下一步

环境创建成功后:

1. ✅ 激活环境: `conda activate github-actions-ci-tool`
2. ✅ 运行工具: `python create_github_actions_ci.py`
3. ✅ 查看使用说明: 参阅 `GitHub_Actions_CI_Tool_README.md`

## 相关文件

- `environment.yml` - Conda 环境配置文件
- `conda_setup.sh` - 自动设置脚本
- `github_actions_tool_requirements.txt` - pip 依赖文件 (备用)
- `GitHub_Actions_CI_Tool_README.md` - 完整使用说明

---

**提示**: 每次打开新的终端窗口时,都需要重新激活 conda 环境。如果希望自动激活,可以将 `conda activate github-actions-ci-tool` 添加到 `.bashrc` 或 `.zshrc` 文件中 (不推荐,除非只在特定项目中使用)。