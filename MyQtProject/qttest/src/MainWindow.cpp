#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    
    // 现在可以通过 ui-> 访问 .ui 文件中定义的控件
    // 根据 MainWindow.ui 文件，有一个名为 pushButton 的按钮和一个名为 label 的标签
    // 连接按钮的点击信号到槽函数
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    
    setWindowTitle("Qt Application");
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

void MainWindow::onButtonClicked()
{
    QMessageBox::information(this, "提示", "按钮被点击了！\n这是一个 Qt 应用程序示例。");
}
