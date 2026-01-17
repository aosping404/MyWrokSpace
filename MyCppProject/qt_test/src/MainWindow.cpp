#include "MainWindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPushButton>
#include <QMouseEvent>
#include <QtMath>

// Shape 基类
class Shape {
public:
    Shape() : selected(false) {}
    virtual ~Shape() {}
    virtual void draw(QPainter& painter) = 0;
    virtual bool contains(const QPoint& point) = 0;
    virtual QVector<QPoint> getHandles() = 0;
    virtual void moveBy(const QPoint& delta) = 0;
    virtual void resizeHandle(int handleIndex, const QPoint& newPos) = 0;
    bool selected;
};

// Circle 派生类
class Circle : public Shape {
public:
    Circle(int x, int y, int r) : center(x, y), radius(r) {}
    void draw(QPainter& painter) override {
        painter.drawEllipse(center, radius, radius);
        if (selected) {
            painter.setPen(QPen(Qt::red, 2));
            painter.drawEllipse(center, radius, radius);
            // 绘制角点manhattanLength
            QVector<QPoint> handles = getHandles();
            for (const QPoint& h : handles) {
                painter.fillRect(h.x() - 5, h.y() - 5, 10, 10, Qt::blue);
            }
        }
    }
    bool contains(const QPoint& point) override {
        return (point - center).manhattanLength() <= radius;
    }
    QVector<QPoint> getHandles() override {
        return {center + QPoint(radius, 0), center + QPoint(0, radius), center + QPoint(-radius, 0), center + QPoint(0, -radius)};
    }
    void moveBy(const QPoint& delta) override {
        center += delta;
    }
    void resizeHandle(int handleIndex, const QPoint& newPos) override {
        QPoint dir = newPos - center;
        int dx = dir.x();
        int dy = dir.y();
        double len = qSqrt((double)dx * dx + dy * dy);
        radius = qMax(10, (int)len);
    }
private:
    QPoint center;
    int radius;
};

// Square 派生类
class Square : public Shape {
public:
    Square(int x, int y, int w, int h) : topLeft(x, y), width(w), height(h) {}
    void draw(QPainter& painter) override {
        painter.drawRect(topLeft.x(), topLeft.y(), width, height);
        if (selected) {
            painter.setPen(QPen(Qt::red, 2));
            painter.drawRect(topLeft.x(), topLeft.y(), width, height);
            QVector<QPoint> handles = getHandles();
            for (const QPoint& h : handles) {
                painter.fillRect(h.x() - 5, h.y() - 5, 10, 10, Qt::blue);
            }
        }
    }
    bool contains(const QPoint& point) override {
        return point.x() >= topLeft.x() && point.x() <= topLeft.x() + width &&
        point.y() >= topLeft.y() && point.y() <= topLeft.y() + height;
    }
    QVector<QPoint> getHandles() override {
        return {topLeft, topLeft + QPoint(width, 0), topLeft + QPoint(width, height), topLeft + QPoint(0, height)};
    }
    void moveBy(const QPoint& delta) override {
        topLeft += delta;
    }
    void resizeHandle(int handleIndex, const QPoint& newPos) override {
        if (handleIndex == 0) { // top-left
            width = qMax(10, width - (newPos.x() - topLeft.x()));
            height = qMax(10, height - (newPos.y() - topLeft.y()));
            topLeft = newPos;
        } else if (handleIndex == 1) { // top-right
            width = qMax(10, newPos.x() - topLeft.x());
            height = qMax(10, height - (newPos.y() - topLeft.y()));
            topLeft.setY(newPos.y());
        } else if (handleIndex == 2) { // bottom-right
            width = qMax(10, newPos.x() - topLeft.x());
            height = qMax(10, newPos.y() - topLeft.y());
        } else if (handleIndex == 3) { // bottom-left
            width = qMax(10, width - (newPos.x() - topLeft.x()));
            height = qMax(10, newPos.y() - topLeft.y());
            topLeft.setX(newPos.x());
        }
    }
private:
    QPoint topLeft;
    int width, height;
};

// Triangle 派生类
class Triangle : public Shape {
public:
    Triangle(const QPoint& p1, const QPoint& p2, const QPoint& p3) : points({p1, p2, p3}) {}
    void draw(QPainter& painter) override {
        painter.drawPolygon(points.data(), 3);
        if (selected) {
            painter.setPen(QPen(Qt::red, 2));
            painter.drawPolygon(points.data(), 3);
            QVector<QPoint> handles = getHandles();
            for (const QPoint& h : handles) {
                painter.fillRect(h.x() - 5, h.y() - 5, 10, 10, Qt::blue);
            }
        }
    }
    bool contains(const QPoint& point) override {
        // 简化：检查点是否在三角形的边界框内
        int minX = qMin(qMin(points[0].x(), points[1].x()), points[2].x());
        int maxX = qMax(qMax(points[0].x(), points[1].x()), points[2].x());
        int minY = qMin(qMin(points[0].y(), points[1].y()), points[2].y());
        int maxY = qMax(qMax(points[0].y(), points[1].y()), points[2].y());
        return point.x() >= minX && point.x() <= maxX && point.y() >= minY && point.y() <= maxY;
    }
    QVector<QPoint> getHandles() override {
        return points;
    }
    void moveBy(const QPoint& delta) override {
        for (QPoint& p : points) p += delta;
    }
    void resizeHandle(int handleIndex, const QPoint& newPos) override {
        points[handleIndex] = newPos;
    }
private:
    QVector<QPoint> points;
};

// DrawingWidget 类
class DrawingWidget : public QWidget {
public:
    DrawingWidget(QWidget *parent = nullptr) : QWidget(parent), dragging(false), resizing(false), selectedShape(nullptr), handleIndex(-1) {}
    ~DrawingWidget() {
        qDeleteAll(shapes);
    }
    void addShape(Shape* shape) {
        shapes.append(shape);
        update();
    }
protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::black, 2));
        
        for (Shape* shape : shapes) {
            shape->draw(painter);
        }
    }
    void mousePressEvent(QMouseEvent *event) override {
        QPoint pos = event->pos();
        // 检查是否点击了角点
        if (selectedShape) {
            QVector<QPoint> handles = selectedShape->getHandles();
            for (int i = 0; i < handles.size(); ++i) {
                if (QRect(handles[i].x() - 5, handles[i].y() - 5, 10, 10).contains(pos)) {
                    resizing = true;
                    handleIndex = i;
                    lastPos = pos;
                    return;
                }
            }
        }
        // 检查是否点击了图形
        for (Shape* shape : shapes) {
            if (shape->contains(pos)) {
                if (selectedShape) selectedShape->selected = false;
                selectedShape = shape;
                shape->selected = true;
                dragging = true;
                lastPos = pos;
                update();
                return;
            }
        }
        // 点击空白，取消选择
        if (selectedShape) {
            selectedShape->selected = false;
            selectedShape = nullptr;
            update();
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override {
        if (dragging && selectedShape) {
            QPoint delta = event->pos() - lastPos;
            selectedShape->moveBy(delta);
            lastPos = event->pos();
            update();
        } else if (resizing && selectedShape) {
            selectedShape->resizeHandle(handleIndex, event->pos());
            update();
        }
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        dragging = false;
        resizing = false;
        handleIndex = -1;
    }
private:
    QList<Shape*> shapes;
    Shape* selectedShape;
    bool dragging;
    bool resizing;
    int handleIndex;
    QPoint lastPos;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_drawingWidget(nullptr)
    , m_btnCircle(nullptr)
    , m_btnCircle2(nullptr)
    , m_btnCircle3(nullptr)
{
    setupUI();
    setWindowTitle("线条绘制演示");
    resize(400, 400);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onCreateCircle()
{
    m_drawingWidget->addShape(new Circle(100, 100, 50));
}

void MainWindow::onCreateSquare()
{
    m_drawingWidget->addShape(new Square(200, 200, 100, 100));
}

void MainWindow::onCreateTriangle()
{
    m_drawingWidget->addShape(new Triangle(QPoint(300, 300), QPoint(250, 400), QPoint(350, 400)));
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_drawingWidget = new DrawingWidget(this);
    m_btnCircle = new QPushButton("创建一个圆", this); 
    m_btnCircle2 = new QPushButton("创建一个正方形", this); 
    m_btnCircle3 = new QPushButton("创建一个三角形", this); 
    m_btnCircle->setFixedSize(180, 60); 
    m_btnCircle2->setFixedSize(220, 60);
    m_btnCircle3->setFixedSize(220, 60);

    connect(m_btnCircle, &QPushButton::clicked, this, &MainWindow::onCreateCircle);
    connect(m_btnCircle2, &QPushButton::clicked, this, &MainWindow::onCreateSquare);
    connect(m_btnCircle3, &QPushButton::clicked, this, &MainWindow::onCreateTriangle);

    layout->addWidget(m_btnCircle); 
    layout->addWidget(m_btnCircle2); 
    layout->addWidget(m_btnCircle3); 
    layout->addWidget(m_drawingWidget);
}
