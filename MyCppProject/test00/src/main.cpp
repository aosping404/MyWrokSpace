#include <iostream>
#include <iomanip>
#include "main.h"
#define LOG(x) std::cout<<x<<std::endl
void increment(int *value){
    (*value)++;
}
class Player {
public:
    int playerX{},playerY{},playerSpeed{};
    void Move(int speed,int xa,int ya){
    playerX += xa*speed;
    playerY += ya*speed;
}
};

struct Date{
    int year {};
    int month {};
    int day {};
    /*
        一旦初始化了常量类类型对象后，就不能修改它的数据成员
        因为违反了const属性。并且你也会惊讶的发现即使是print()
        这种不尝试修改成员变量的函数，在外部进行调用仍然也无法编译
        这是因为print成员函数本身没有声明为const。编译器不允许对
        常量对象调用非const成员函数。

        为解决上述问题，需要将print()成为const成员函数。来保证不会i修改对象
        或调用任何非const成员函数，因为它们可能会修改对象。
        下面在定义print函数时紧接着将const关键字附加到了函数原型中
        在参数列表后，在函数体之前。
    */
    void print() const 
    { 
        std::cout<<year<<'/'<<month<<'/'<<day<<"\n";
    }

    /*----------------------------------------------------
     * 把成员函数声明为 const 的两大理由：
     * 1. 告诉编译器“本函数不会修改对象的任何成员”
     * 2. 允许常量对象调用它，例如
     *    const Date d{2025,11,18};
     *    d.print();          // 如果 print 不是 const，这里会编译失败
     *--------------------------------------------------*/
};
/*========================================================
 *  插值查找（Interpolation Search）
 *  前提：数组必须有序且值分布“较为均匀”
 *  思想：不是像二分查找那样简单取中点，而是
 *        按“目标值在区间内的相对位置”去估算下标
 *  时间复杂度：
 *        均匀数据 → 平均 O(log log n)
 *        极端倾斜 → 退化为 O(n)
 *------------------------------------------------------
 * @param arr    升序整型数组
 * @param n      数组元素个数
 * @param target 待查找值
 * @return       找到则返回下标，否则返回 -1
 *------------------------------------------------------*/
int interpolation_search(int arr[], int n, int target) {
    int low{0}, high{n - 1};

    while (low <= high && target >= arr[low] && target <= arr[high]) {
        if (low == high) {
            if (arr[low] == target) return low;
            return -1;
        }

        // 使用浮点插值避免截断
        int pos{low + static_cast<int>(
            (static_cast<double>(high - low) / (arr[high] - arr[low])) *
            (target - arr[low])
        )};

        if (arr[pos] == target) return pos;
        if (arr[pos] < target) low = pos + 1;
        else high = pos - 1;
    }

    return -1;
}
int main() {
#if 0
    std::cout << greet() << std::endl;
    // 跳转语句
    int n {5};
    mylabel:
    std::cout << n <<"\n";
    n--;
    if (n>0) goto mylabel;
    // Hello, World!
    // 5
    // 4
    // 3
    // 2
    // 1
#endif
#if 0
    std::cout << std::left; // 左对齐
    std::cout << std::setw(16) << "bool:" << sizeof(bool) << " bytes\n";
    std::cout << std::setw(16) << "char:" << sizeof(char) << " bytes\n";
    std::cout << std::setw(16) << "short:" << sizeof(short) << " bytes\n";
    std::cout << std::setw(16) << "int:" << sizeof(int) << " bytes\n";
    std::cout << std::setw(16) << "long:" << sizeof(long) << " bytes\n";
    std::cout << std::setw(16) << "long long:" << sizeof(long long) << " bytes\n";
    std::cout << std::setw(16) << "float:" << sizeof(float) << " bytes\n";
    std::cout << std::setw(16) << "double:" << sizeof(double) << " bytes\n";
    std::cout << std::setw(16) << "long double:" << sizeof(long double) << " bytes\n";
    // bool:           1 bytes
    // char:           1 bytes
    // short:          2 bytes
    // int:            4 bytes
    // long:           8 bytes
    // long long:      8 bytes
    // float:          4 bytes
    // double:         8 bytes
    // long double:    16 bytes
#endif
#if 0
    unsigned int u{ 2 };
	signed int s{ 3 };

	std::cout << u - s << '\n'; // 2 - 3 = 4294967295
#endif
    //我们都知道2-3是-1，但-1不能表示为无符号整数，因此我们得到溢出和以下结果：
    //4294967295
    //当无符号整数重复递减1（使用–运算符）并小于0时，会发生另一种常见的不需要的重新计数。当学习循环时，您将看到这样的例子。
#if 0
    signed int s { -1 };
    unsigned int u { 1 };

    if (s < u) // -1 被转换为 4294967295,  4294967295 < 1 结果 false
        std::cout << "-1 is less than 1\n";
    else
        std::cout << "1 is less than -1\n"; // 这条语句执
#endif
    //当混合有符号整数和无符号整数时，可能会导致意外的行为。
    //在C++中，如果数学运算（例如算术或比较）有一个有符号整数和一个无符号整数，
    //则有符号整数通常会转换为无符号整数。因此，结果将是无符号的。例如：
#if 0
    srand(time(NULL));
    //获取某个区间的随机数公式： rand()%[（尾+1）-头]+头
    int res = rand()%113+33;  //如33-145的随机数生成，头是33，尾是145
    std::cout<<res<<"\n";
#endif
    // char arr[10] = {"hel\0lo"}; 
    // std::cout<<arr<<"\n";
#if 0
/*
    12 45 95 16 43 28
    遍历数组查找对应数组下标
*/
    int arr[] = {12,45,28,16,43,28};
    int l = sizeof(arr)/sizeof(*arr);
    int f = 28;
    for(int i = 0;i<l;i++){
        if(arr[i]==f) {
            std::cout<<i<<"\n";
        }
    }
#endif
#if 0
/*
    int arr[] = {1,2,5,9,12,45,55,78,95,116,224,336,448,999,2000,6666,7777,9999};
    二分查找
*/
    int arr[] = {1,2,10,12,45,55,78,95,116,224,336,448,999,2000,6666,7777};
    int l{sizeof(arr)/sizeof(*arr)};
    int f{448};
    int left{0};
    int right = l-1;
    int mid = l/2;
    std::cout<<*(arr+left)<<"\n"<<*(arr+right)<<"\n";
    while(left<=right){
        if(*(arr+mid)>f) {
            right = mid-1;
        }
        else if((*(arr+mid))<f){
            left = mid+1;
        }
        else {
            std::cout<<mid<<"\n";
            break;
        }
        mid = left+(right-left)/2;
    }
#endif
#if 0
/*
插值查找法  ！！！插值查找必须带浮点运算和安全检查
公式：pos=low+（high-low)/(array[high]-array[low])*(target-array[low])
直接使用该公式会导致两个问题：
    ❌ 问题 1：整数除法截断
        C++ 中整数除法会截断小数部分，导致公式失真。
        举例：(right - left) / (arr[right] - arr[left])
        如果 (right - left) < (arr[right] - arr[left])，这个除法结果是 0，整个公式就退化成：
        pos = left + 0 * (...) = left
    ❌ 问题 2：未检查除零风险
        如果 arr[right] == arr[left]（即区间所有值相等），除以零会导致未定义行为（UB）。
*/
    int arr[] = {1,2,5,9,12,45,55,78,95,116,224,336,448,999,2000,6666,7777,9999};
    int len{sizeof(arr) / sizeof(*arr)};
    int target{9999};

    int index{interpolation_search(arr, len, target)};
    if (index != -1)
        std::cout << "Found at index: " << index << "\n";
    else
        std::cout << "Not found\n";
#endif
#if 0
    Date today {2020,10,14};
    today.day = 16;
    today.print();
#endif
#if 0
    int a{};
    LOG(a);
    increment(&a);
    LOG(a);
#endif
    Player king;
    LOG(king.playerX);
    LOG(king.playerY);
    LOG(king.playerSpeed);
    king.Move(10,2,2);
    LOG(king.playerX);
    LOG(king.playerY);
    return 0;
}