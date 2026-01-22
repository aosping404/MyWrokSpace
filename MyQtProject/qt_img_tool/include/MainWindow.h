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
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
private:
    Ui::MainWindow *ui; // UI 指针，用于访问 .ui 文件中定义的控件
    QPixmap m_originalPixmap; // 始终保存加载时的最高画质原图
    QPixmap m_displayPixmap;  // 当前显示的图片（缩放后的）
    bool m_isDrawing = false; // 是否正在拖动鼠标画框
    QPoint m_startPoint;      // 鼠标按下的起点
    QPoint m_endPoint;        // 鼠标移动的终点
    QRect m_rect;             // 最终生成的矩形
    double m_zoomFactor = 1.0;
protected:
    void paintEvent(QPaintEvent *event) override;
    //鼠标事件处理函数
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};
