#include "CubeWidget.h"
#include <QPainter>
#include <QColor>
#include <QImage>
#include <QLinearGradient>
#include <QResizeEvent>
#include <algorithm>

CubeWidget::CubeWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: black;");
    setMinimumSize(400, 400);
    
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, [this]() {
        m_angle += 0.02f;
        update();
    });
    m_timer->start(16);  // 约 60 FPS
}

CubeWidget::~CubeWidget()
{
}

void CubeWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void CubeWidget::paintEvent(QPaintEvent *event)
{
    int w = width();
    int h = height();
    int pixelSize = 4;
    
    int lowResW = (w + pixelSize - 1) / pixelSize;
    int lowResH = (h + pixelSize - 1) / pixelSize;
    
    QImage lowResImage(lowResW, lowResH, QImage::Format_ARGB32);
    lowResImage.fill(Qt::black);
    
    QPainter lowResPainter(&lowResImage);
    lowResPainter.setRenderHint(QPainter::Antialiasing, false);

    // ============ 定义顶点 ============
    Vec3 vertices[8] = {
        Vec3(-1, -1,  1),  Vec3( 1, -1,  1),  Vec3( 1,  1,  1),  Vec3(-1,  1,  1),
        Vec3(-1, -1, -1),  Vec3( 1, -1, -1),  Vec3( 1,  1, -1),  Vec3(-1,  1, -1)
    };

    struct Face { int indices[4]; };
    Face faces[6] = {
        {{0, 1, 2, 3}},  {{1, 5, 6, 2}},  {{5, 4, 7, 6}},
        {{4, 0, 3, 7}},  {{3, 2, 6, 7}},  {{4, 5, 1, 0}}
    };

    // ============ 场景设置 ============
    QPointF projected[8];
    QPointF shadowProjected[8];
    Vec3 transformed[8];
    float vertexIntensities[8];
    
    float centerX = lowResW / 2.0f;
    float centerY = lowResH / 2.0f;
    float fov = 150.0f;
    
    // [设置 1] 地板高度：立方体底部是 -1，地板设在 -1.5 留点空隙，更有悬浮感
    float floorY = -1.5f; 

    // [设置 2] 光源位置：拉远、拉高 (模拟右上方的摄影棚主光)
    // x=6 (右), y=10 (高), z=-8 (前) -> 影子会稳定地落在左后方地板上
    Vec3 lightPos(6.0f, 10.0f, -8.0f);

    // ============ 2. 顶点变换与光照计算 ============
    for (int i = 0; i < 8; ++i) {
        float x = vertices[i].x;
        float y = vertices[i].y;
        float z = vertices[i].z;

        // --- 旋转 ---
        float cosA = cos(m_angle);
        float sinA = sin(m_angle);
        float nx = x * cosA - z * sinA;
        float nz = x * sinA + z * cosA;

        float cosB = cos(m_angle * 0.7f);
        float sinB = sin(m_angle * 0.7f);
        float ny = y * cosB - nz * sinB;
        nz = y * sinB + nz * cosB;

        // --- 世界坐标 (推远 5.0) ---
        transformed[i] = Vec3(nx, ny, nz + 5.0f);

        // --- 光照计算 (修正后的稳定光照) ---
        // 光线向量
        Vec3 toLight = Vec3(lightPos.x - transformed[i].x, 
                            lightPos.y - transformed[i].y, 
                            lightPos.z - transformed[i].z);
        toLight.normalize();

        // 法线 (仅旋转，无位移)
        Vec3 vNormal(nx, ny, nz); 
        vNormal.normalize();

        float intensity = Vec3::dot(vNormal, toLight);
        intensity = std::max(0.0f, intensity);
        
        // 柔和的光照曲线
        intensity = 0.15f + 0.85f * intensity;
        vertexIntensities[i] = std::min(1.0f, intensity);

        // --- 物体投影 ---
        if (transformed[i].z > 0.1f) {
            float scale = fov / transformed[i].z;
            projected[i] = QPointF(centerX + nx * scale, centerY - ny * scale);
        }

        // --- [核心修改] 真实的影子投影计算 ---
        // 我们需要计算：从光源出发，经过顶点，打在地板平面 (y = floorY) 上的交点
        // 利用相似三角形原理 (Ray-Plane Intersection)
        
        // 1. 相对高度差
        float dy = transformed[i].y - lightPos.y; // 肯定是负数，因为物体在光源下面
        float targetDy = floorY - lightPos.y;     // 光源到地板的垂直距离

        // 2. 比例因子 t
        if (std::abs(dy) > 1e-4) {
            float t = targetDy / dy; // 射线延长的倍数
            
            // 3. 计算在地板上的 X 和 Z 坐标
            float sx = lightPos.x + (transformed[i].x - lightPos.x) * t;
            float sz = lightPos.z + (transformed[i].z - lightPos.z) * t;
            
            // 4. 将地板上的 3D 点投影到屏幕 (透视)
            if (sz > 0.1f) {
                float sScale = fov / sz;
                shadowProjected[i] = QPointF(centerX + sx * sScale, centerY - floorY * sScale);
            }
        }
    }

    // ============ 3. 面排序 (画家算法) ============
    struct RenderFace {
        int index;
        float zDepth;
    };
    RenderFace renderOrder[6];
    Vec3 faceNormals[6];

    for (int i = 0; i < 6; ++i) {
        int i0 = faces[i].indices[0];
        int i1 = faces[i].indices[1];
        int i2 = faces[i].indices[2];

        // 计算面法线 (用于背面剔除)
        Vec3 v1 = transformed[i1] - transformed[i0];
        Vec3 v2 = transformed[i2] - transformed[i0];
        faceNormals[i] = Vec3::cross(v1, v2);

        renderOrder[i].index = i;
        // Z-Sort 深度排序
        renderOrder[i].zDepth = (transformed[i0].z + transformed[i1].z + 
                                 transformed[i2].z + transformed[faces[i].indices[3]].z) / 4.0f;
    }

    std::sort(renderOrder, renderOrder + 6, [](const RenderFace &a, const RenderFace &b) {
        return a.zDepth > b.zDepth; // 从远到近绘制
    });

    // ============ 绘制影子 ============
    // 影子颜色：深灰偏冷，更有质感
    lowResPainter.setPen(Qt::NoPen);
    lowResPainter.setBrush(QColor(25, 25, 30)); 

    for (int idx = 0; idx < 6; ++idx) {
        int i = renderOrder[idx].index;
        if (faceNormals[i].z > 0) continue; 

        QPolygonF shadowPoly;
        for (int j = 0; j < 4; ++j) {
            shadowPoly << shadowProjected[faces[i].indices[j]];
        }
        lowResPainter.drawPolygon(shadowPoly);
    }

    // ============ 绘制实体 ============
    lowResPainter.setPen(QPen(QColor(30, 30, 30), 1)); 

    for (int idx = 0; idx < 6; ++idx) {
        int i = renderOrder[idx].index;
        if (faceNormals[i].z > 0) continue; 

        // 获取光照强度
        float ints[4];
        QPointF pts[4];
        for(int k=0; k<4; k++) {
            ints[k] = vertexIntensities[faces[i].indices[k]];
            pts[k] = projected[faces[i].indices[k]];
        }

        // 寻找最亮和最暗点用于渐变
        int minIdx = 0, maxIdx = 0;
        for(int k=1; k<4; k++) {
            if(ints[k] < ints[minIdx]) minIdx = k;
            if(ints[k] > ints[maxIdx]) maxIdx = k;
        }

        QLinearGradient gradient(pts[minIdx], pts[maxIdx]);
        
        // 颜色映射：使用略微偏冷的金属灰
        int vMin = static_cast<int>(ints[minIdx] * 255);
        int vMax = static_cast<int>(ints[maxIdx] * 255);
        
        gradient.setColorAt(0.0, QColor(vMin, vMin, vMin));
        gradient.setColorAt(1.0, QColor(vMax, vMax, vMax));

        lowResPainter.setBrush(gradient);
        
        QPolygonF poly;
        for (int j = 0; j < 4; ++j) poly << pts[j];
        
        lowResPainter.drawPolygon(poly);
    }

    lowResPainter.end();

    // ============ 上屏 ============
    QPainter screenPainter(this);
    screenPainter.fillRect(rect(), Qt::black);
    screenPainter.setRenderHint(QPainter::Antialiasing, false);
    screenPainter.drawImage(rect(), lowResImage); 

    // Debug Info
    screenPainter.setRenderHint(QPainter::Antialiasing, true);
    screenPainter.setPen(QColor(100, 255, 100));
    screenPainter.setFont(QFont("Courier", 10));
    screenPainter.drawText(20, 30, "Light Source: Fixed (Studio High-Right)");
    screenPainter.drawText(20, 50, "Shadow: Ray-Plane Intersection");
}