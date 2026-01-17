# YCXC++_01

C++ 项目

## 项目结构

```
YCXC++_01/
├── .vscode/          # VSCode 配置文件
│   ├── tasks.json    # 构建任务配置
│   ├── launch.json   # 调试配置
│   └── c_cpp_properties.json  # IntelliSense 配置
├── include/          # 头文件目录
│   └── main.h
├── src/              # 源代码目录
│   └── main.cpp
└── README.md
```

## 使用方法

1. 在 VSCode 中打开项目文件夹
2. 按 `F5` 或点击播放按钮进行调试
3. 按 `Ctrl+Shift+B` 进行构建

## 编译和运行

### 使用 VSCode 任务
- 构建活动文件: `Ctrl+Shift+B` (选择 "C/C++: g++ build active file")
- 构建整个项目: `Ctrl+Shift+B` (选择 "C/C++: g++ build project")

### 使用命令行
```bash
# 编译单个文件
g++ -g src/main.cpp -o main -Iinclude

# 运行
./main
```

## 调试

1. 在代码中设置断点（点击行号左侧）
2. 按 `F5` 开始调试
3. 使用调试控制面板进行单步调试
