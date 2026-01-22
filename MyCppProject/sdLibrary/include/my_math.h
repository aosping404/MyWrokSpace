// my_math.h
#ifndef MY_MATH_H
#define MY_MATH_H

// 针对动态库的导出宏（Windows 必须，Linux 可选但建议）
#ifdef _WIN32
    #ifdef BUILD_DLL
        #define API __declspec(dllexport)
    #else
        #define API __declspec(dllimport)
    #endif
#else
    #define API
#endif

int API add(int a, int b);

#endif