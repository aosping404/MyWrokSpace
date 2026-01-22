#pragma once

#include <QMainWindow>
#include <QList>
#include <QPointF>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QtMath>

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
};

class lineTraking : public QWidget{
    Q_OBJECT

public:
    lineTraking(QWidget *parent);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void updatePosition();
    void drawArrow(QPainter *painter, QPointF pos, qreal angle);
    QList<QPointF> points;
    QPointF currentPos;
    int targetIndex;
    QTimer *timer;
};