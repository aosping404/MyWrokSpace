#include "ui_logingPage.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "logingPage.h"
#include <QMessageBox>
#include <QSettings>
#include <QString>
#include <QDir>
#include <QApplication>
logingPage::logingPage(QWidget *parent)
    : QDialog(parent)
    , logingui(new Ui::logingPage) // 初始化 UI 指针
{
    logingui->setupUi(this); // 加载 UI 布局
    setWindowTitle("登录窗口");
    setModal(true); // 设置为模态窗口
}

logingPage::~logingPage()
{
    delete logingui; // 释放 UI 指针
}
//取消-关闭窗口
void logingPage::on_pushButton_clicked(){
    reject(); //返回 QDialog::Rejected
}

//登陆
void logingPage::on_pushButton_2_clicked(){
    QString configPath = QApplication::applicationDirPath() + "/setup.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    
    QString m_user_name = settings.value("user").toString();
    QString m_user_pw = settings.value("password").toString();

    if(m_user_name.isEmpty() || m_user_pw.isEmpty()){
        QMessageBox::warning(this, "错误", "无法读取配置文件！\n请确保 setup.ini 文件存在。");
        return;
    }

    user_name = logingui->lineEdit->text();
    user_pw = logingui->lineEdit_2->text();
    
    if(user_name == m_user_name && user_pw == m_user_pw){
        accept(); //关闭对话框，返回 QDialog::Accepted
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}
