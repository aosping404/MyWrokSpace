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
    
    // 加载 QSS 样式表
    void loadStyleSheet(const QString &styleName);

private slots:
    void onButtonClicked();
    void onStyleChanged(int index); // 样式切换槽函数
    void onOpenIkeaShowcase(); // 打开 IKEA 展示窗口

private:
    Ui::MainWindow *ui; // UI 指针，用于访问 .ui 文件中定义的控件
    void initializeStyleComboBox(); // 初始化样式下拉框
};
