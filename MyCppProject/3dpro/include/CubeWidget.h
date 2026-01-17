#pragma once

#include <QWidget>
#include <QTimer>
#include <cmath>

// 快速反向平方根算法 (Quake III Arena 经典算法)
inline float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;                    // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);          // what the fuck?
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y));   // 1st iteration
    y = y * (threehalfs - (x2 * y * y));   // 2nd iteration

    return y;
}

// 三维向量结构体
struct Vec3 {
    float x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    
    // 使用魔数进行快速归一化
    void normalize() {
        float lenSq = x*x + y*y + z*z;
        // 防止除零
        if (lenSq < 1e-6f) return; 
        
        float invLen = Q_rsqrt(lenSq);
        x *= invLen;
        y *= invLen;
        z *= invLen;
    }
    
    // 点积
    static float dot(const Vec3 &a, const Vec3 &b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
    
    // 叉积
    static Vec3 cross(const Vec3 &a, const Vec3 &b) {
        return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }
    
    // 减法运算符
    Vec3 operator-(const Vec3 &other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
};

class CubeWidget : public QWidget {
    Q_OBJECT
public:
    explicit CubeWidget(QWidget *parent = nullptr);
    ~CubeWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    float m_angle = 0.0f;
    QTimer *m_timer = nullptr;
};
