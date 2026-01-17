#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QPainter>
#include <QVector>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 设置 UI
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}
//绘版实现
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing); //启用抗锯齿和图像平滑
    QRect r = rect(); //获取绘图区域
    QColor WhiteColor(255, 255, 255);
    painter.fillRect(r, WhiteColor); //填充白色背景
    if(m_showBackground){
        m_showLogo = false; // 关闭 Logo 显示
        QPixmap pixmap(":/background/1111.png");
        if(!pixmap.isNull()){
            painter.drawPixmap(r, pixmap);
        }
    }
    else { 
        QRect rect(180,10,1200,800);
        QColor RedColor(255, 255, 200);
        painter.fillRect(rect, RedColor);
    }
    painter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    for(const auto &line : lines){
        for(int i = 1; i < line.size(); ++i){
            painter.drawLine(line[i - 1], line[i]);
        }
    }
    if (m_showLogo) {
        QPixmap logoPixmap(":/background/logo_simple.png");
        if (!logoPixmap.isNull()) {
            painter.drawPixmap(200, 30, 64, 64, logoPixmap);
            painter.drawPixmap(400, 60, 128, 128, logoPixmap);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;
        lines.append(QVector<QPoint>()); // 添加一个新的笔画
        lines.last().append(event->pos()); // 记录起始点
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isDrawing) {
        lines.last().append(event->pos()); // 记录移动点
        update(); // 触发重绘
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
    }
}
void MainWindow::on_pushButton_3_clicked()
{
    m_showBackground = true;
    update(); // 触发重绘
}
void MainWindow::on_pushButton_4_clicked(){
    //贴一个图片在画板 ":/background/logo_simple.png"
    m_showBackground = false;
    m_showLogo = true;        // 开启 Logo 显示 
    update(); // 触发重绘
}