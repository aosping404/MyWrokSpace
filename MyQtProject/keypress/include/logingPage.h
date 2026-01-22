#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class logingPage; } // 必须与 .ui 文件中的类名一致
QT_END_NAMESPACE

class logingPage : public QDialog{
    Q_OBJECT
public:
    explicit logingPage(QWidget *parent = nullptr);
    ~logingPage();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
private:
    Ui::logingPage *logingui;
    QString user_name;
    QString user_pw;
};