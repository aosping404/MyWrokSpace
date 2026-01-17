#include <iostream>
#include <vector>
#include <string>
#include "main.h"
#include "stu.h"
#include "myCout.h"


int main()
{
#if 0
    LOG("\n========== 复数运算演示 (实数和虚数) ==========\n");
    
    // 创建复数对象
    Complex c1(3, 4);      // 3 + 4i
    Complex c2(1, 2);      // 1 + 2i
    double num = 5;         // 实数
    
    LOG("\n初始复数:");
    LOG("c1 = " << c1);
    LOG("c2 = " << c2);
    LOG("num = " << num);
    
    // 加法运算
    LOG("\n[1] 加法运算:");
    LOG("c1 + c2 = " << (c1 + c2));
    LOG("c1 + 5 = " << (c1 + num));
    LOG("5 + c2 = " << (num + c2));
    
    // 减法运算
    LOG("\n[2] 减法运算:");
    LOG("c1 - c2 = " << (c1 - c2));
    LOG("c1 - 5 = " << (c1 - num));
    LOG("5 - c2 = " << (num - c2));
    
    // 乘法运算
    LOG("\n[3] 乘法运算:");
    Complex mult = c1 * c2;
    LOG("c1 * c2 = " << mult << " (计算: (3+4i)*(1+2i) = (3-8)+(6+4)i = -5+10i)");
    LOG("c1 * 5 = " << (c1 * num));
    LOG("5 * c2 = " << (num * c2));
    
    // 除法运算
    LOG("\n[4] 除法运算:");
    Complex div = c1 / c2;
    LOG("c1 / c2 = " << div);
    LOG("c1 / 5 = " << (c1 / num));
    LOG("5 / c2 = " << (num / c2));
    
    // 共轭复数
    LOG("\n[5] 共轭复数:");
    LOG("c1 = " << c1 << ", 共轭 = " << c1.conjugate());
    LOG("c2 = " << c2 << ", 共轭 = " << c2.conjugate());
    
    // 模（绝对值）
    LOG("\n[6] 复数的模:");
    LOG("|c1| = " << c1.modulus() << " (计算: √(3²+4²) = √25 = 5)");
    LOG("|c2| = " << c2.modulus() << " (计算: √(1²+2²) = √5 ≈ 2.236)");
    
    // 比较运算
    LOG("\n[7] 比较运算:");
    Complex c3(3, 4);
    if (c1 == c3) {
        LOG("c1 == c3 (都是 3 + 4i)");
    }
    if (c1 != c2) {
        LOG("c1 != c2");
    }
    
    // 赋值运算
    LOG("\n[8] 赋值运算:");
    Complex c4;
    c4 = c1;
    LOG("c4 = c1, 结果: c4 = " << c4);
    Complex c5;
    c5 = 10;  // 赋值实数
    LOG("c5 = 10, 结果: c5 = " << c5);
    
    LOG("\n========== 复数运算演示结束 ==========\n");
#endif
    myCout mycout;
    mycout<<"Hello World!"<<"123\n";
    (mycout.operator<<("Hello World!")).operator<<("123\n");
    std::cout<<"Hello World!"<<"123\n";
    (std::cout.operator<<("Hello World!")).operator<<("123\n");
    return 0;
}
