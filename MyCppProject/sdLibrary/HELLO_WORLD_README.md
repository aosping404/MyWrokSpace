# 语音合成 Hello World 示例

这是一个使用科大讯飞AIKit SDK进行语音合成的简单示例程序。

## 功能说明

- 将文本 "Hello World，你好世界" 合成为语音文件
- 输出PCM格式的音频文件：`hello_world.pcm`

## 使用前准备

### 1. 配置SDK参数

编辑 `src/hello_world_tts.cpp` 文件，填入你的科大讯飞SDK凭证：

```cpp
AIKIT_Configurator::builder()
    .app()
        .appID("你的appID")           // 请替换为你的appID
        .apiSecret("你的apiSecret")   // 请替换为你的apiSecret
        .apiKey("你的apiKey")         // 请替换为你的apiKey
        .workDir("./")
    ...
```

### 2. 确保资源文件存在

确保SDK资源文件在正确位置：
- `Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/aisound/resource/`

## 编译和运行

### 方法1：使用CMake

```bash
# 1. 创建构建目录
mkdir -p build
cd build

# 2. 配置项目
cmake ..

# 3. 编译
make

# 4. 运行
./bin/hello_world_tts
```

### 方法2：直接使用g++（参考）

```bash
g++ -std=c++11 -o hello_world_tts src/hello_world_tts.cpp \
    -Iinclude \
    -ILinux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/include \
    -LLinux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/libs \
    -laikit \
    -Wl,-rpath=Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/libs

# 设置库路径
export LD_LIBRARY_PATH=./Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/libs:$LD_LIBRARY_PATH

# 运行
./hello_world_tts
```

## 播放生成的音频文件

生成的 `hello_world.pcm` 是PCM格式的音频文件，可以使用以下方式播放：

### 使用ffplay（推荐）

```bash
ffplay -f s16le -ar 16000 -ac 1 hello_world.pcm
```

参数说明：
- `-f s16le`: 采样格式为16位小端
- `-ar 16000`: 采样率为16000Hz
- `-ac 1`: 单声道

### 转换为WAV格式（可选）

如果需要转换为WAV格式，可以使用ffmpeg：

```bash
ffmpeg -f s16le -ar 16000 -ac 1 -i hello_world.pcm hello_world.wav
```

然后就可以用任何音频播放器播放 `hello_world.wav` 了。

## 代码说明

### 主要函数

1. **InitSDK()**: 初始化SDK，配置appID、apiKey等参数
2. **TextToSpeech()**: 执行文本转语音合成
3. **OnOutput()**: 回调函数，接收合成的音频数据
4. **OnEvent()**: 回调函数，处理合成事件（如合成完成）
5. **OnError()**: 回调函数，处理错误

### 可调整的参数

在 `TextToSpeech()` 函数中可以调整以下参数：

```cpp
paramBuilder->param("vcn", "xiaoyan", strlen("xiaoyan"));  // 发音人
paramBuilder->param("speed", 50);      // 语速：0-100
paramBuilder->param("pitch", 50);      // 音调：0-100
paramBuilder->param("volume", 50);     // 音量：0-100
```

支持的发音人：
- `xiaoyan`: 小燕（中文女声）
- `catherine`: Catherine（英文女声）
- 更多发音人请参考SDK文档

## 常见问题

### 1. 运行时找不到库文件

如果出现 `libaikit.so: cannot open shared object file` 错误：

```bash
export LD_LIBRARY_PATH=./bin:$LD_LIBRARY_PATH
```

或者确保库文件已复制到可执行文件所在目录。

### 2. 找不到资源文件

确保 `aisound/resource/` 目录在可执行文件所在目录，CMake会自动复制。

### 3. SDK初始化失败

- 检查appID、apiKey、apiSecret是否正确
- 检查网络连接（首次使用需要联网认证）
- 查看日志文件了解详细错误信息

## 下一步

- 修改文本内容，合成不同的语音
- 尝试不同的发音人和参数
- 将PCM转换为其他音频格式
- 集成到你的应用程序中
