#include <iostream>
#include <vector>
#include <string>
#include "main.h"
#include <cmath>
using namespace std;
//带inline头文件的快速分区伽马函数算法
/*
#include <cmath>
#include <limits>

inline double round4(double x) noexcept
{
    constexpr double scale = 10000.0;
    return std::round(x * scale) / scale;
}

// 0.5 ≤ x ≤ 5.5 的多项式逼近
// 最大误差 ~1e-4，round4 后 4 位小数正确
inline double gamma_small(double x) noexcept
{
    // 系数已预先 round4 化，省一次截断
    constexpr double c0 =  1.0001;
    constexpr double c1 = -0.5749;
    constexpr double c2 =  0.9518;
    constexpr double c3 = -0.6998;
    constexpr double c4 =  0.1541;
    double t = x - 1.0;                 // 以 1.0 为中心
    double t2 = round4(t * t);
    double t3 = round4(t2 * t);
    double t4 = round4(t3 * t);
    return round4(round4(round4(c0 + c1*t) + round4(c2*t2)) +
                  round4(round4(c3*t3) + round4(c4*t4)));
}

// 5.5 < x ≤ 70 的 Stirling 一阶修正
inline double gamma_stirling(double x) noexcept
{
    double ln_x  = std::log(x);
    double x_inv = round4(1.0 / x);                 // 截 4 位
    double corr  = round4(1.0 / (12.0 * x));        // 1/(12x)
    double ln_g  = round4(0.5 * std::log(6.2832))   // 2π≈6.2832
                 - round4(0.5 * ln_x)
                 + round4(x * (ln_x - 1.0))
                 + corr;
    return round4(std::exp(ln_g));
}

// 任意 x > 0 的入口
inline double gamma_quick4(double x) noexcept
{
    if (x < 0.5)                 // 反射公式 Γ(x)=π/(sin(πx)Γ(1−x))
    {
        double s = std::sin(round4(3.1416 * x));
        return round4(3.1416 / round4(s * gamma_quick4(1.0 - x)));
    }
    if (x <= 5.5) return gamma_small(x);
    if (x <= 70)  return gamma_stirling(x);
    // x > 70 主项已足够
    return std::exp(0.5 * std::log(6.2832 / x) + x * (std::log(x) - 1.0));
}
*/
//伽马展开 如5.5 Γ(5.5)=4.5·3.5·2.5·1.5·0.5·Γ(0.5)
inline double round5(double x) noexcept
{
    constexpr double scale = 100000.0;
    return std::round(x * scale) / scale;
}

//编程2题
double func(int n){
    double a{1.5};
    double x{0.5};
    double k{round5(0.0078125)};
    double num{round5(0.75-k-0.03125)};
    for(int i{4};i<=n;i++){
        cout<<"-------i="<<i<<"-------"<<endl;
        a = a+1;
        cout<<"a="<<a<<endl;
        k = round5(k*a*x/i);
        if(k==0) {
            num -= k;
            cout<<"第"<<i<<"次循环终止"<<endl;
            break;
        }
        cout<<"k="<<k<<endl;
        num -= k;
        cout<<"num="<<num<<endl;
        cout<<"----------------"<<endl;
    }
    return num;
}

bool is_prime(int n){
    int i{};
    for (i = 2; i <= n - 1; i++) {
        if (n % i == 0) {
        break;
    }
    }
    if (i >= n) {
        return true;
    } else {
        return false;
    }
}

int newbase(unsigned long long value, int base, int d[])
{
    int len = 0;
    while (value) {
        d[len++] = value % base;
        value /= base;
    }
    // 处理 value 原为 0 的情况
    if (len == 0) d[len++] = 0;
    // d[0] 是个位，逆序成高位在前
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        int t = d[i]; d[i] = d[j]; d[j] = t;
    }
    return len;   // 返回用了多少位
}

int main()
{
#if 0
    int n{10000};
    cout<<func(n)<<endl;
    cout<<"精确到小数4位"<<endl;
#endif
#if 0
//编程3题：输出斐波那契数列（1,1,2,3,5,8,13,21... ）的前15项的和
//斐波那契数列定义 fn：f(1)=f(2)=1,f(i)=f(i-1)+f(i-2)(i>=3)
    int a[15]{1,1};
    int num = 2;
    for(int i{3};i<14;i++){
        a[i]=a[i-1]+a[i-2];
        cout<<a[i]<<endl;
        num += a[i];
    }
    cout<<num<<endl;
#endif
#if 0
//编程4题：因式分解
    int primes{};
    int N=98604899;
    int N_flog{N};
    int i{};
    while(primes<my_sqrt(N_flog)){
        primes++;
        if(is_prime(primes)) {
            if(N_flog%primes==0){
                N_flog = N_flog/primes;
                cout<<N_flog<<endl;
                i++;
            }
        }
    }
#endif
//编程5题：求数组n元素中，所有偶数的积 减 所有奇数的积
#if 0
    int n[12] = { 112,14, 57,6, 18,5, 19,11, 12,13,24,31 };
    long long k{1};
    long long d{1};
    for(int i{};i<12;i++){
        if(is_prime(n[i])) {
            k *= n[i];
        }
        else {
            d *= n[i];
        }
    }
    cout<<d-k<<endl;
#endif
//编程6题：10进制123456转成6进制 
//10进制以内版
#if 0
    int i{123456};
    int iarr[100];
    for(int n{};n<newbase(i,6,iarr);n++){
        cout<<iarr[n];
    }
    cout<<"\n";
#endif
    int i{1},b{3},c{4};
    int *p1 = &i;
    cout<<static_cast<int *>(p1)<<endl;
    return 0;
}
