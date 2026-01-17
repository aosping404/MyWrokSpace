#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // 必须与 .ui 文件中的类名一致
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 可以在这里添加槽函数声明，例如按钮点击事件的处理函数
    // void onButtonClicked();
    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_action_4_triggered();
private:
    Ui::MainWindow *ui; // UI 指针，用于访问 .ui 文件中定义的控件
};
