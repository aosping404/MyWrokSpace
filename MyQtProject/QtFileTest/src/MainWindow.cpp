#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
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
void MainWindow::on_action_triggered(){
    QApplication::quit();
}
//实现新文件，打开文件，保存文件的槽函数
void MainWindow::on_action_2_triggered(){
    //新文件 （简化实现）
    ui->textEdit->clear();
}
void MainWindow::on_action_3_triggered(){
    //保存文件
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", "", "所有文件 (*.*);;文本文件 (*.txt)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << ui->textEdit->toPlainText(); // 假设 UI 中有一个 QTextEdit 控件用于显示文件内容
            file.close();
        } else {
            QMessageBox::warning(this, "错误", "无法保存文件！");
        }
    }
}
void MainWindow::on_action_4_triggered(){
        //打开文件
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", "", "所有文件 (*.*);;文本文件 (*.txt)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            ui->textEdit->setPlainText(content); // 假设 UI 中有一个 QTextEdit 控件用于显示文件内容
            file.close();
        } else {
            QMessageBox::warning(this, "错误", "无法打开文件！");
        }
    }
}
