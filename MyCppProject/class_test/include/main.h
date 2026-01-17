#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <vector>
#include <cstdint>
#include <cstring>   // 仅为了 memcpy，属于 <cmath> 的依赖体系内
// 示例函数声明 魔法数字求倒根函数 以及牛顿迭代法求根函数 来自于《雷神之锤3》
/*
Q_rsqrt 仍然是“零依赖、可移植、比标准库快数倍”的经典技巧；
但在现代编译器 + 硬件 rsqrt 指令面前，它更多是历史趣味和教学价值，
除非你需要手动控制迭代次数或无硬件开方的嵌入式场景。
*/
inline float my_rsqrt(float x) noexcept
{
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    float y = x;
    float xhalf = 0.5f * y;
    std::uint32_t i;
    std::memcpy(&i, &y, sizeof(float));
    i = 0x5F3759DFu - (i >> 1);
    std::memcpy(&y, &i, sizeof(float));
    y = y * (1.5f - xhalf * y * y);   // 一次牛顿迭代
    return y;
}

inline double my_sqrt(double x) noexcept
{
    if (x == 0.0) return 0.0;
    double z = x;          // 初始值
    for (int i = 0; i < 5; ++i)          // 5 次牛顿迭代足够
        z = 0.5 * (z + x / z);
    return z;
}

#endif // MAIN_H