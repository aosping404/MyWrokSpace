#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    lineTraking *traker = new lineTraking(this);
    setCentralWidget(traker);
    setWindowTitle("循迹");
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

lineTraking::lineTraking(QWidget *parent)
    : QWidget(parent)
{
    targetIndex = 0;
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&lineTraking::updatePosition);
}
void lineTraking::mousePressEvent(QMouseEvent *event) {
    points.append(event->position());
    if (points.size() >= 2 && !timer->isActive()) {
        // 起点，初始化位置
        if (targetIndex == 0) {
            currentPos = points[0];
            targetIndex = 1;
        }
        timer->start(16);
    }
    update();
}

void lineTraking::updatePosition() {
    if (targetIndex >= points.size()) {
        timer->stop();
        return;
    }

    QPointF target = points[targetIndex];
    QLineF line(currentPos, target);
    qreal speed = 2.0; 

    if (line.length() <= speed) {
        currentPos = target;
        targetIndex++;

        if (targetIndex >= points.size()) {
            timer->stop();
        }
    } else { 
        qreal angle = qDegreesToRadians(-line.angle());
        currentPos += QPointF(speed * qCos(angle), speed * qSin(angle));
    }
    update();
}
// 绘图事件
void lineTraking::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 画路径
    if (points.size() > 1) {
        painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
        for(int i=0; i<points.size()-1; ++i) 
            painter.drawLine(points[i], points[i+1]);
    }

    // 画箭头
    if (!currentPos.isNull()) {
        qreal angle = 0;
        if (targetIndex < points.size()) {
            angle = -QLineF(currentPos, points[targetIndex]).angle();
        }
        drawArrow(&painter, currentPos, angle);
    }
}

void lineTraking::drawArrow(QPainter *painter, QPointF pos, qreal angle) {
    painter->save();
    painter->translate(pos);
    painter->rotate(angle);
    painter->setBrush(Qt::red);
    static const QPointF triangle[] = {{10,0}, {-10,-6}, {-10,6}};
    painter->drawPolygon(triangle, 3);
    painter->restore();
}