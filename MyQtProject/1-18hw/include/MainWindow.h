#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QPaintEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // 必须与 .ui 文件中的类名一致
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:

private:
    Ui::MainWindow *ui; // UI 指针，用于访问 .ui 文件中定义的控件
};
