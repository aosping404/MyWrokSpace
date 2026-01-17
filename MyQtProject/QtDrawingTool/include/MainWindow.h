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
    
private:
    Ui::MainWindow *ui; // UI 指针，用于访问 .ui 文件中定义的控件
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<QVector<QPoint>> lines; // 存储所有笔画的点列表
    bool isDrawing = false; // 标记是否正在绘制
    bool m_showBackground = false; // 标记是否显示背景
    bool m_showLogo = false;
private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
};


