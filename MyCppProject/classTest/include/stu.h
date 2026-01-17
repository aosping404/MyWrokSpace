#include<string>
#include <iostream>
#include <cmath>
#define LOG(x) std::cout<<x<<std::endl

// 复数类：处理实数和虚数
class Complex {
private:
    double real;      // 实部
    double imag;      // 虚部（虚根）
    
public:
    // 构造函数
    Complex();
    Complex(double r);              // 仅实部
    Complex(double r, double i);    // 实部和虚部
    
    // 获取成员
    double getReal() const { return real; }
    double getImag() const { return imag; }
    
    // 打印复数
    void print() const;
    
    // 运算符重载 - 加法
    Complex operator+(const Complex& other) const;
    Complex operator+(double num) const;  // 与实数相加
    friend Complex operator+(double num, const Complex& c);
    
    // 运算符重载 - 减法
    Complex operator-(const Complex& other) const;
    Complex operator-(double num) const;  // 减去实数
    friend Complex operator-(double num, const Complex& c);
    
    // 运算符重载 - 乘法
    Complex operator*(const Complex& other) const;
    Complex operator*(double num) const;  // 与实数相乘
    friend Complex operator*(double num, const Complex& c);
    
    // 运算符重载 - 除法
    Complex operator/(const Complex& other) const;
    Complex operator/(double num) const;  // 除以实数
    friend Complex operator/(double num, const Complex& c);
    
    // 比较运算符
    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;
    
    // 赋值运算符
    Complex& operator=(const Complex& other);
    Complex& operator=(double num);
    
    // 求共轭复数
    Complex conjugate() const;
    
    // 求模（绝对值）
    double modulus() const;
    
    // 流输出运算符
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};

// 原有的 Student 类
class Student {
public:
    int index;
    int age;
    std::string name;
    void print();
    Student();
    Student(int _index,int _age,const char* name);
    Student(int age, const char* name);
    Student(const Student& student_copy);
    ~Student();
private:
    int id{};
    int getAutoId();
};