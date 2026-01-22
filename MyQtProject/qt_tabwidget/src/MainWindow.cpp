#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "dialogstudentinput.h"
#include "MainWindow.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setColumnWidth(0,60);
    ui->tableWidget->setColumnWidth(1,120);
    ui->tableWidget->setColumnWidth(2,80);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,100);
    ui->tableWidget->setColumnWidth(5,100);
    ui->tableWidget->setHorizontalHeaderLabels({"学号","姓名","年龄","数学","语文","英语"});
    ui->tableWidget->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableWidget->setSortingEnabled(true);
    load();
    setWindowTitle("Qt Application");
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

//新增
void MainWindow::on_pushButton_clicked(){
    DialogStudentInput dlg;
    Student stu={0," ",0,0,0,0};
    if(dlg.exec()==1){
        dlg.getData(stu);
        int rowount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowount+1);

        ui->tableWidget->setItem(rowount,0,new QTableWidgetItem(QString::number(stu.index)));
        ui->tableWidget->setItem(rowount,1,new QTableWidgetItem(stu.name));
        ui->tableWidget->setItem(rowount,2,new QTableWidgetItem(QString::number(stu.age)));
        ui->tableWidget->setItem(rowount,3,new QTableWidgetItem(QString::number(stu.math)));
        ui->tableWidget->setItem(rowount,4,new QTableWidgetItem(QString::number(stu.chinese)));
        ui->tableWidget->setItem(rowount,5,new QTableWidgetItem(QString::number(stu.english)));
        save();
    }
}
//修改
void MainWindow::on_pushButton_2_clicked(){
    int row=ui->tableWidget->currentRow();
    if(row!=-1){
        Student stu;
        stu.index=ui->tableWidget->item(row,0)->text().toInt();
        stu.name=ui->tableWidget->item(row,1)->text();
        stu.age=ui->tableWidget->item(row,2)->text().toInt();
        stu.math=ui->tableWidget->item(row,3)->text().toDouble();
        stu.chinese=ui->tableWidget->item(row,4)->text().toDouble();
        stu.english=ui->tableWidget->item(row,5)->text().toDouble();
        DialogStudentInput dlg;
        dlg.setData(stu);
        if(dlg.exec()==1){
            dlg.getData(stu);
            ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(stu.index)));
            ui->tableWidget->setItem(row,1,new QTableWidgetItem(stu.name));
            ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(stu.age)));
            ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::number(stu.math)));
            ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::number(stu.chinese)));
            ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString::number(stu.english)));
            save();
        }
    }
}

//删除
void MainWindow::on_pushButton_3_clicked(){
    int row = ui->tableWidget->currentRow();
    if(row!=-1){
        ui->tableWidget->removeRow(row);
        save();
    }
}
void MainWindow::save(){
    QString filename = QApplication::applicationDirPath()+"/student.txt";
    QJsonArray array;
    int rowcount=ui->tableWidget->rowCount();
    for(int row=0;row<rowcount;row++){
        Student stu;
        stu.index=ui->tableWidget->item(row,0)->text().toInt();
        stu.name=ui->tableWidget->item(row,1)->text();
        stu.age=ui->tableWidget->item(row,2)->text().toInt();
        stu.math=ui->tableWidget->item(row,3)->text().toDouble();
        stu.chinese=ui->tableWidget->item(row,4)->text().toDouble();
        stu.english=ui->tableWidget->item(row,5)->text().toDouble();
        QJsonObject JsonStu;
        JsonStu.insert("index",stu.index);
        JsonStu.insert("name",stu.name);
        JsonStu.insert("age",stu.age);
        JsonStu.insert("math",stu.math);
        JsonStu.insert("chinese",stu.chinese);
        JsonStu.insert("english",stu.english);
        array.append(JsonStu);
    }
    QJsonDocument doc(array);
    QByteArray date=doc.toJson();
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly)){
        file.write(date);
    }
}
void MainWindow::load(){
    QString filename = QApplication::applicationDirPath()+"/student.txt";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray data=file.readAll();
        QJsonDocument doc=QJsonDocument::fromJson(data);
        QJsonArray array=doc.array();
        ui->tableWidget->setRowCount(array.count());//设置网络的总行数
        for(int i{};i<array.count();i++){
            QJsonObject jsonstu=array[i].toObject();
            Student stu;
            stu.index=jsonstu.value("index").toInt();
            stu.name=jsonstu.value("name").toString();
            stu.age=jsonstu.value("age").toInt();
            stu.math=jsonstu.value("math").toDouble();
            stu.chinese=jsonstu.value("chinese").toDouble();
            stu.english=jsonstu.value("english").toDouble();

            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(stu.index)));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(stu.name));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(stu.age)));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(stu.math)));
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(stu.chinese)));
            ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(stu.english)));
        }
    }
}