#pragma once

#include <QMainWindow>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // 必须与 .ui 文件中的类名一致
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QTimer m_timer;
    int m_count;
    ~MainWindow();

private slots:
    void timeout();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_horizontalSlider_actionTriggered(int action);
private:
    Ui::MainWindow *ui; // UI 指针，用于访问 .ui 文件中定义的控件
};
