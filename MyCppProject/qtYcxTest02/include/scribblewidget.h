#ifndef SCRIBBLEWIDGET_H
#define SCRIBBLEWIDGET_H

#include <QWidget>
#include <QLine>
#include <QVector>

class scribbleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit scribbleWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *e)   override;
    void mouseMoveEvent(QMouseEvent *e)    override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *)         override;

private:
    QVector<QLine> lines_;   // 已固定的直线
    QLine          current_; // 正在拖拽的临时直线
    bool           drawing_ = false;
};

#endif // SCRIBBLEWIDGET_H