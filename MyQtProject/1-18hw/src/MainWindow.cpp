#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <QGridLayout>
#include <QPainter>
#include <QPen>     // 边框
#include <QBrush>   // 填充
#include <QFont>    // 字体
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    QElapsedTimer *runTimer = new QElapsedTimer();
    QGridLayout *layout = new QGridLayout(ui->centralwidget);
    runTimer->start();
    layout->addWidget(ui->label, 0, 0, Qt::AlignCenter);
    ui->label->setStyleSheet("color: red; background: transparent; font-weight: bold; font-size: 18px;");
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setFixedSize(50, 50);
    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout,this,[=](){
        qint64 ms =runTimer->elapsed();
        ui->label->setText(QString::number(ms / 1000));
        update();
    });
    timer->start(1000);
    setWindowTitle("Qt Application");
}




MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);

    //填充
    painter.setBrush(Qt::white);

    QRect labelRect = ui->label->geometry();
    int radius = qMax(labelRect.width(), labelRect.height()) / 2+5; 
    QPoint center = labelRect.center();

    painter.drawEllipse(center, radius, radius);
}