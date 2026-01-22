#include "ui_dialogstudentinput.h"// 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "dialogstudentinput.h"
#include <QMessageBox>

DialogStudentInput::DialogStudentInput(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogStudentInput) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
}

DialogStudentInput::~DialogStudentInput()
{
    delete ui; // 释放 UI 指针
}

void DialogStudentInput::setData(Student &stu){
    ui->lineEditName->setText(stu.name);
    ui->lineEditIndex->setText(QString::number(stu.index));
    ui->lineEditAge->setText(QString::number(stu.age));
    ui->lineEditMath->setText(QString::number(stu.math));
    ui->lineEditChinese->setText(QString::number(stu.chinese));
    ui->lineEditEnglish->setText(QString::number(stu.english));
}
void DialogStudentInput::getData(Student &stu){
    stu.name=ui->lineEditName->text();
    stu.index=ui->lineEditIndex->text().toInt();
    stu.age=ui->lineEditAge->text().toInt();
    stu.math=ui->lineEditMath->text().toDouble();
    stu.chinese=ui->lineEditChinese->text().toDouble();
    stu.english=ui->lineEditEnglish->text().toDouble();
}
void DialogStudentInput::on_pushButton_2_clicked(){
    close();
}
void DialogStudentInput::on_pushButton_clicked(){
    done(1);
}

