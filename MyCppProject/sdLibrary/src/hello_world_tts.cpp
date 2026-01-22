/*
 * 科大讯飞语音合成 Hello World 示例
 * 功能：将文本"Hello World"合成为语音文件
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <atomic>
#include <unistd.h>

#include "aikit_biz_api.h"
#include "aikit_constant.h"
#include "aikit_biz_config.h"

using namespace std;
using namespace AIKIT;

// 全局变量
FILE* output_file = nullptr;
static const char* ABILITY = "ece9d3c90";  // 能力ID（固定值，不要改成appID）
static std::atomic_bool tts_finished(false);
static bool sdk_initialized = false;  // SDK初始化状态

// 输出回调函数：接收合成的音频数据
void OnOutput(AIKIT_HANDLE* handle, const AIKIT_OutputData* output) {
    printf("收到音频数据，key: %s, 长度: %d\n", output->node->key, output->node->len);
    
    if (output->node->value && output_file != nullptr) {
        // 将音频数据写入文件
        fwrite(output->node->value, sizeof(char), output->node->len, output_file);
    }
}

// 事件回调函数：处理合成事件
void OnEvent(AIKIT_HANDLE* handle, AIKIT_EVENT eventType, const AIKIT_OutputEvent* eventValue) {
    printf("事件类型: %d\n", eventType);
    
    if (eventType == AIKIT_Event_End) {
        printf("语音合成完成！\n");
        tts_finished = true;
    }
}

// 错误回调函数：处理错误
void OnError(AIKIT_HANDLE* handle, int32_t err, const char* desc) {
    printf("错误码: %d, 描述: %s\n", err, desc ? desc : "未知错误");
}

// 初始化SDK
void InitSDK() {
    // 配置SDK参数
    // ⚠️ 重要：必须使用"离线引擎托管平台"的认证信息，不是"Websocket服务接口"的认证信息！
    // 请登录离线引擎托管平台创建应用并获取正确的 appID、apiKey 和 apiSecret
    // 详细说明请查看项目根目录的"认证信息说明.md"文件
    AIKIT_Configurator::builder()
        .app()
            .appID("")           // ⚠️ 请填入离线引擎托管平台的appID（不是开放平台的appID）
            .apiSecret("")       // ⚠️ 请填入离线引擎托管平台的apiSecret
            .apiKey("")          // ⚠️ 请填入离线引擎托管平台的apiKey
            .workDir("./")       // 工作目录
        .auth()
            .authType(0)         // 认证类型
        .log()
            .logLevel(LOG_LVL_INFO)
            .logPath("./");      // 日志路径
    
    int ret = AIKIT_Init();
    if (ret != 0) {
        printf("AIKIT_Init 失败，错误码: %d\n", ret);
        printf("\n请检查以下问题：\n");
        printf("  1. appID、apiKey、apiSecret 是否正确\n");
        printf("  2. 网络连接是否正常（首次使用需要联网认证）\n");
        printf("  3. 应用是否已在科大讯飞开放平台正确配置\n");
        printf("  4. 错误码 2003 表示'未发现应用'，请确认appID是否正确\n");
        sdk_initialized = false;
        return;  // 返回而不是退出，避免崩溃
    }
    
    // 注册回调函数
    AIKIT_Callbacks cbs = {OnOutput, OnEvent, OnError};
    AIKIT_RegisterAbilityCallback(ABILITY, cbs);
    
    sdk_initialized = true;
    printf("SDK初始化成功！\n");
}

// 语音合成函数
void TextToSpeech(const char* text, const char* output_filename) {
    // 检查SDK是否已初始化
    if (!sdk_initialized) {
        printf("错误：SDK未初始化，无法进行语音合成。\n");
        return;
    }
    
    AIKIT_ParamBuilder* paramBuilder = nullptr;
    AIKIT_DataBuilder* dataBuilder = nullptr;
    AIKIT_HANDLE* handle = nullptr;
    AiText* aiText = nullptr;
    int ret = 0;
    tts_finished = false;
    
    // 打开输出文件
    output_file = fopen(output_filename, "wb");
    if (output_file == nullptr) {
        printf("无法创建输出文件: %s\n", output_filename);
        return;
    }
    printf("输出文件: %s\n", output_filename);
    
    // 创建参数构建器并设置参数
    paramBuilder = AIKIT_ParamBuilder::create();
    paramBuilder->param("vcn", "xiaoyan", strlen("xiaoyan"));  // 发音人：小燕（中文）
    // paramBuilder->param("vcn", "catherine", strlen("catherine"));  // 发音人：Catherine（英文）
    paramBuilder->param("speed", 50);      // 语速：0-100，默认50
    paramBuilder->param("pitch", 50);      // 音调：0-100，默认50
    paramBuilder->param("volume", 50);     // 音量：0-100，默认50
    paramBuilder->param("textEncoding", "UTF-8", strlen("UTF-8"));  // 文本编码
    
    // 启动会话
    ret = AIKIT_Start(ABILITY, AIKIT_Builder::build(paramBuilder), nullptr, &handle);
    printf("AIKIT_Start 返回码: %d\n", ret);
    if (ret != 0) {
        printf("启动会话失败！\n");
        goto cleanup;
    }
    
    // 创建数据构建器并设置文本
    dataBuilder = AIKIT_DataBuilder::create();
    aiText = AiText::get("text")->data(text, strlen(text))->valid();
    dataBuilder->payload(aiText);
    
    // 写入文本数据
    ret = AIKIT_Write(handle, AIKIT_Builder::build(dataBuilder));
    printf("AIKIT_Write 返回码: %d\n", ret);
    if (ret != 0) {
        printf("写入文本失败！\n");
        goto cleanup;
    }
    
    printf("正在合成语音...\n");
    
    // 等待合成完成
    while (!tts_finished) {
        usleep(10000);  // 休眠10毫秒
    }
    
    // 结束会话
    ret = AIKIT_End(handle);
    printf("AIKIT_End 返回码: %d\n", ret);
    
cleanup:
    // 清理资源
    if (paramBuilder != nullptr) {
        delete paramBuilder;
    }
    if (dataBuilder != nullptr) {
        delete dataBuilder;
    }
    if (output_file != nullptr) {
        fclose(output_file);
        output_file = nullptr;
    }
}

int main() {
    printf("========================================\n");
    printf("科大讯飞语音合成 Hello World 示例\n");
    printf("========================================\n\n");
    
    // 初始化SDK
    InitSDK();
    
    // 如果SDK初始化失败，直接返回
    if (!sdk_initialized) {
        printf("\n程序退出：SDK初始化失败，请检查配置后重试。\n");
        return 1;
    }
    
    // 合成文本
    const char* text = "Hello World，你好世界";
    const char* output_file = "hello_world.pcm";
    
    printf("\n要合成的文本: %s\n", text);
    TextToSpeech(text, output_file);
    
    // 检查是否生成了文件
    FILE* check_file = fopen(output_file, "r");
    if (check_file != nullptr) {
        fclose(check_file);
        printf("\n✓ 语音文件已生成: %s\n", output_file);
        printf("注意：生成的是PCM格式的音频文件，可以使用ffplay播放：\n");
        printf("  ffplay -f s16le -ar 16000 -ac 1 %s\n", output_file);
    } else {
        printf("\n✗ 警告：未能生成语音文件，请检查SDK配置和网络连接。\n");
    }
    
    // 清理SDK资源
    if (sdk_initialized) {
        AIKIT_UnInit();
    }
    
    printf("\n程序结束。\n");
    
    return 0;
}
