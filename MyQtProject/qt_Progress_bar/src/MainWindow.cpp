#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    
    // 现在可以通过 ui-> 访问 .ui 文件中定义的控件
    // 根据 MainWindow.ui 文件，有一个名为 pushButton 的按钮和一个名为 label 的标签
    // 连接按钮的点击信号到槽函数
    connect(&m_timer,&QTimer::timeout,this,&MainWindow::timeout);
    setWindowTitle("Qt Application");
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}


void MainWindow::timeout(){
    m_count++;
    ui->progressBar->setValue(m_count);
    if(m_count==100){
        m_timer.stop();
    }
}

void MainWindow::on_pushButton_clicked(){
        m_count++;
    ui->progressBar->setValue(0);
    m_timer.start(100);
}

void MainWindow::on_pushButton_2_clicked(){
    ui->progressBar->setStyleSheet("QProgressBar{text-align: center;"
        "background-color: #e2e3e4;"
        "border: 1px solid #999999;"
        "border-radius: 5px;}"
        "QProgressBar::chunk{background-color: #995fff;"
        "border-radius: 5px;}");
    ui->progressBar->repaint();
}
void MainWindow::on_pushButton_3_clicked(){
    ui->progressBar->setStyleSheet("QProgressBar{text-align: center;"
        "background-color: #e2e3e4;"
        "border: 0px solid #e2e3e4;"
        "border-radius: 5px;}"
        "QProgressBar::chunk{background-color: #995fff; "
        "border-radius: 5px;}");
    ui->progressBar->repaint();
}
void MainWindow::on_pushButton_4_clicked(){
    ui->progressBar->setStyleSheet("QProgressBar{text-align: center;"
        "background-color: #e2e3e4;"
        "border: 0px solid #e2e3e4;"
        "border-radius: 3px;}"
        "QProgressBar::chunk{background-color:#995fff;"
        "border-radius: 3px; "
        "width: 10px;"
        "margin: 0.5px;}");
    ui->progressBar->repaint();
}
void MainWindow::on_pushButton_5_clicked(){
    ui->progressBar->setStyleSheet("QProgressBar{height:24px; "
        "text-align: center; "
        "font-size: 14px; "
        "color: white; "
        "border-radius: 5px; "
        "background: #e2e3e4;}"
        "QProgressBar::chunk{border-radius: 5px;"
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,"
        "stop:0 #995fff,"
        "stop:1 #6900ff);}");
    ui->progressBar->repaint();
}

void MainWindow::on_horizontalSlider_actionTriggered(int action){
    int value=ui->horizontalSlider->value();
    ui->progressBar_2->setValue(value);
}