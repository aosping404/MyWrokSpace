#include "scribblewidget.h"
#include <QPainter>
#include <QMouseEvent>

scribbleWidget::scribbleWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
}

void scribbleWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        drawing_ = true;
        current_ = QLine(e->pos(), e->pos()); // 起点终点相同
        update();
    }
}

void scribbleWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (drawing_) {
        current_.setP2(e->pos()); // 只改终点
        update();
    }
}

void scribbleWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (drawing_ && e->button() == Qt::LeftButton) {
        current_.setP2(e->pos());
        lines_.append(current_);  // 真正保存
        drawing_ = false;
        update();
    }
}

void scribbleWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black, 2));

    // 画已固定的直线
    for (const QLine &l : lines_)
        p.drawLine(l);

    // 画正在拖拽的临时直线
    if (drawing_)
        p.drawLine(current_);
}