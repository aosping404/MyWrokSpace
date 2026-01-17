#include "stu.h"

// ========== Complex 类实现 ==========

// 构造函数
Complex::Complex() : real(0), imag(0) {}

Complex::Complex(double r) : real(r), imag(0) {}

Complex::Complex(double r, double i) : real(r), imag(i) {}

// 打印复数
void Complex::print() const {
    if (imag >= 0) {
        LOG(real << " + " << imag << "i");
    } else {
        LOG(real << " - " << -imag << "i");
    }
}

// 加法运算符 - 复数+复数
Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

// 加法运算符 - 复数+实数
Complex Complex::operator+(double num) const {
    return Complex(real + num, imag);
}

// 加法运算符 - 实数+复数（友元函数）
Complex operator+(double num, const Complex& c) {
    return Complex(c.real + num, c.imag);
}

// 减法运算符 - 复数-复数
Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

// 减法运算符 - 复数-实数
Complex Complex::operator-(double num) const {
    return Complex(real - num, imag);
}

// 减法运算符 - 实数-复数（友元函数）
Complex operator-(double num, const Complex& c) {
    return Complex(num - c.real, -c.imag);
}

// 乘法运算符 - 复数*复数
// (a+bi)(c+di) = (ac-bd) + (ad+bc)i
Complex Complex::operator*(const Complex& other) const {
    double new_real = real * other.real - imag * other.imag;
    double new_imag = real * other.imag + imag * other.real;
    return Complex(new_real, new_imag);
}

// 乘法运算符 - 复数*实数
Complex Complex::operator*(double num) const {
    return Complex(real * num, imag * num);
}

// 乘法运算符 - 实数*复数（友元函数）
Complex operator*(double num, const Complex& c) {
    return Complex(c.real * num, c.imag * num);
}

// 除法运算符 - 复数/复数
// (a+bi)/(c+di) = [(a+bi)(c-di)]/[(c+di)(c-di)] = [(ac+bd)+(bc-ad)i]/(c²+d²)
Complex Complex::operator/(const Complex& other) const {
    double denom = other.real * other.real + other.imag * other.imag;
    if (denom == 0) {
        LOG("错误: 除数为零!");
        return Complex(0, 0);
    }
    double new_real = (real * other.real + imag * other.imag) / denom;
    double new_imag = (imag * other.real - real * other.imag) / denom;
    return Complex(new_real, new_imag);
}

// 除法运算符 - 复数/实数
Complex Complex::operator/(double num) const {
    if (num == 0) {
        LOG("错误: 除以零!");
        return Complex(0, 0);
    }
    return Complex(real / num, imag / num);
}

// 除法运算符 - 实数/复数（友元函数）
Complex operator/(double num, const Complex& c) {
    return Complex(num, 0) / c;
}

// 比较运算符
bool Complex::operator==(const Complex& other) const {
    return real == other.real && imag == other.imag;
}

bool Complex::operator!=(const Complex& other) const {
    return !(*this == other);
}

// 赋值运算符
Complex& Complex::operator=(const Complex& other) {
    if (this != &other) {
        real = other.real;
        imag = other.imag;
    }
    return *this;
}

Complex& Complex::operator=(double num) {
    real = num;
    imag = 0;
    return *this;
}

// 求共轭复数 (a+bi的共轭是a-bi)
Complex Complex::conjugate() const {
    return Complex(real, -imag);
}

// 求模（绝对值）|a+bi| = √(a²+b²)
double Complex::modulus() const {
    return std::sqrt(real * real + imag * imag);
}

// 流输出运算符
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    if (c.imag >= 0) {
        os << c.real << " + " << c.imag << "i";
    } else {
        os << c.real << " - " << -c.imag << "i";
    }
    return os;
}

// ========== Student 类实现 ==========

// 静态计数器，用于生成唯一id
static int _id_counter = 0;

// 获取自动ID
int Student::getAutoId() {
    return _id_counter++;
}

// 无参构造函数
Student::Student() : index(0), age(0), name("") {
    id = getAutoId();
    LOG("Student 无参构造函数调用 - ID: " << id);
}

// 有参构造函数 (index, age, name)
Student::Student(int _index, int _age, const char* _name) 
    : index(_index), age(_age), name(_name) {
    id = getAutoId();
    LOG("Student 构造函数调用 - ID: " << id << ", index: " << index 
        << ", age: " << age << ", name: " << name);
}

// 有参构造函数 (age, name)
Student::Student(int _age, const char* _name) 
    : index(0), age(_age), name(_name) {
    id = getAutoId();
    LOG("Student 构造函数调用 - ID: " << id << ", age: " << age 
        << ", name: " << name);
}

// 拷贝构造函数
Student::Student(const Student& student_copy) 
    : index(student_copy.index), age(student_copy.age), name(student_copy.name) {
    id = getAutoId();
    LOG("Student 拷贝构造函数调用 - ID: " << id 
        << ", 复制自ID: " << student_copy.id);
}

// 析构函数
Student::~Student() {
    LOG("Student 析构函数调用 - ID: " << id << ", name: " << name);
}

// 打印学生信息
void Student::print() {
    LOG("=== Student 信息 ===");
    LOG("ID: " << id);
    LOG("Index: " << index);
    LOG("Age: " << age);
    LOG("Name: " << name);
    LOG("====================");
}

