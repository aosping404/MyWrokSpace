#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include "countThread.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPainter>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
    , m_buttonY(60)  // 第一个计数器按钮从Y=60开始
{
    ui->setupUi(this); // 加载 UI 布局
    m_thread.m_mainWindow=this;
    m_thread.start();
    setWindowTitle("Qt 多线程计数器示例");
    resize(400, 400);  // 调整窗口大小以容纳多个按钮
}

MainWindow::~MainWindow()
{
    m_thread.stop();
    // 停止并删除所有线程
    for (CountThread *thread : m_threads) {
        if (thread) {
            thread->requestInterruption();
            thread->wait();
            delete thread;
        }
    }
    
    // 删除所有按钮（线程已停止，可以安全删除）
    for (QPushButton *button : m_countButtons) {
        delete button;
    }
    
    delete ui; // 释放 UI 指针
}

// 增加计数器
void MainWindow::on_pushButton_clicked()
{
    // 创建新的计数器按钮
    QPushButton *countButton = new QPushButton("0", this);
    countButton->setGeometry(110, m_buttonY, 200, 30);
    countButton->setText("0");
    countButton->show();
    
    m_countButtons.append(countButton);
    
    // 创建新的计数器线程
    CountThread *thread = new CountThread(countButton, this);
    m_threads.append(thread);
    
    // 连接信号槽：将线程的计数更新信号连接到按钮的setText槽
    connect(thread, &CountThread::countUpdated, countButton, &QPushButton::setText);
    
    // 启动线程
    thread->start();
    
    // 更新下一个按钮的Y坐标
    m_buttonY += 40;
    
    // 如果按钮太多，调整窗口大小
    if (m_buttonY + 40 > height()) {
        resize(width(), m_buttonY + 60);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QDateTime currentDate = QDateTime::currentDateTime();
    QString dateTimeString = currentDate.toString("yyyy-MM-dd hh:mm:ss.zzz");
    QFont font("微软雅黑",30);
    QPen pen(QColor::fromRgb(255,0,0));
    painter.setPen(pen);
    painter.drawText(rect(),Qt::AlignCenter,dateTimeString);
}

