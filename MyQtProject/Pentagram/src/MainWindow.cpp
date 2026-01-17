#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    
    // 现在可以通过 ui-> 访问 .ui 文件中定义的控件
    // 根据 MainWindow.ui 文件，有一个名为 pushButton 的按钮和一个名为 label 的标签
    // 连接按钮的点击信号到槽函数
    setWindowTitle("Qt Application");
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿

    QPointF center(width() / 2.0, height() / 2.0);
    double r = qMin(width(), height()) / 2.0 - 10; // 半径

    QPainterPath starPath;
    starPath.setFillRule(Qt::WindingFill);
    double angle = -M_PI / 2.0; 
    double step = 4.0 * M_PI / 5.0; 
    
    starPath.moveTo(center.x() + r * qCos(angle), 
                    center.y() + r * qSin(angle));
    // 连线 5 次完成五角星
    for (int i = 1; i <= 5; ++i) {
        angle += step;
        starPath.lineTo(center.x() + r * qCos(angle), 
                        center.y() + r * qSin(angle));
    }
    starPath.closeSubpath(); // 闭合路径

    // 3. 绘制填充
    painter.setPen(Qt::NoPen);            // 不绘制轮廓线
    painter.setBrush(QColor(255, 215, 0)); // 设置金色纯色填充
    painter.drawPath(starPath);
}