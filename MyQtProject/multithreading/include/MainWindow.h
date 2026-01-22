#pragma once

#include <QMainWindow>
#include <QList>
#include <QPushButton>
#include "timethread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // 必须与 .ui 文件中的类名一致
QT_END_NAMESPACE

class CountThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    TimeThread m_thread;
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    Ui::MainWindow *ui; // UI 指针，用于访问 .ui 文件中定义的控件
    QList<QPushButton*> m_countButtons;  // 存储所有计数器按钮
    QList<CountThread*> m_threads;       // 存储所有线程
    int m_buttonY;                       // 下一个按钮的Y坐标
};
