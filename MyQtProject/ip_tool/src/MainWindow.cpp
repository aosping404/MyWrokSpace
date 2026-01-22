#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    
    setWindowTitle("Qt Application");
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

void MainWindow::on_pushButton_clicked() {
    QString ip = ui->lineEdit->text().trimmed();

    QStringList ipList = ip.split('.', Qt::SkipEmptyParts);

    if (ipList.size() == 4) {
        ui->lineEdit_2->setText(ipList[0]);
        ui->lineEdit_3->setText(ipList[1]);
        ui->lineEdit_4->setText(ipList[2]);
        ui->lineEdit_5->setText(ipList[3]);
    } else {
        QMessageBox::warning(this, "错误", "请输入有效的 IP 地址 (例如: 192.168.1.1)");
    }
}
