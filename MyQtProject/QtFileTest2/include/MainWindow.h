#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString m_iniPath; // 存储实际的配置文件路径

    void initIniFile();   // 初始化路径（确保有可写文件）
    void loadSettings();  // 一次性读取
    void saveSettings();  // 一次性保存（在析构或按钮中调用）

protected:
    // 重写关闭事件，实现关闭窗口自动保存
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H