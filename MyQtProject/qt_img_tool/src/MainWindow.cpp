#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    
    // 现在可以通过 ui-> 访问 .ui 文件中定义的控件
    // 根据 MainWindow.ui 文件，有一个名为 pushButton 的按钮和一个名为 label 的标签
    // 连接按钮的点击信号到槽函数
    
    setWindowTitle("Qt Application");
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

void MainWindow::on_pushButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open", "", "Images (*.png *.jpg)");
    if (!fileName.isEmpty()) {
        m_originalPixmap.load(fileName); // 存入原图
        
        // 1. 定义目标范围：窗口宽高的 4/5 (0.8)
        int targetW = this->width() * 0.8;
        int targetH = this->height() * 0.8;
        QSize targetSize(targetW, targetH);

        // 2. 按照原比例计算缩放后的尺寸
        // Qt::KeepAspectRatio 会确保图片不失真地适应 4/5 的窗口区域
        QSize finalSize = m_originalPixmap.size();
        finalSize.scale(targetSize, Qt::KeepAspectRatio);

        // 3. 计算居中位置
        // 矩形左上角 = 窗口中心 - 缩放后尺寸的一半
        int x = (this->width() - finalSize.width()) / 2;
        int y = (this->height() - finalSize.height()) / 2;

        m_rect = QRect(QPoint(x, y), finalSize);

        // 4. 生成显示用的图片并刷新界面
        m_displayPixmap = m_originalPixmap.scaled(m_rect.size(), 
                                                Qt::IgnoreAspectRatio, 
                                                Qt::SmoothTransformation);
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    if (!m_displayPixmap.isNull()) {
        // 如果矩形不为空，我们将图片画在矩形的左上角坐标 (m_rect.x(), m_rect.y())
        if (!m_rect.isNull()) {
            painter.drawPixmap(m_rect.topLeft(), m_displayPixmap);
        } else {
            // 如果还没画矩形，就画在原点
            painter.drawPixmap(0, 0, m_displayPixmap);
        }
    }

    // 绘制那个红色的边框线（可选，提示填充范围）
    if (!m_rect.isNull()) {
        QPen pen(Qt::red, 2, Qt::DashLine); // 使用虚线表示这是填充区
        painter.setPen(pen);
        painter.drawRect(m_rect);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_startPoint = event->pos(); // 记录起点
        m_endPoint = m_startPoint;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDrawing) {
        m_endPoint = event->pos(); // 更新终点
        update(); // 触发重绘，实时看到画框过程
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_isDrawing) {
        m_isDrawing = false;
        m_rect = QRect(m_startPoint, event->pos()).normalized();

        if (!m_originalPixmap.isNull() && !m_rect.isEmpty()) {
            // 核心：永远从 original 缩放，保证最高清晰度
            m_displayPixmap = m_originalPixmap.scaled(m_rect.size(), 
            Qt::IgnoreAspectRatio, 
            Qt::SmoothTransformation);
            update();
        }
    }
}

// 放大 1.2 倍
void MainWindow::on_pushButton_2_clicked() {
    if (m_rect.isNull() || m_originalPixmap.isNull()) return;
    //记录当前中心点
    QPoint center = m_rect.center();
    //计算新尺寸
    int newWidth = m_rect.width() * 1.2;
    int newHeight = m_rect.height() * 1.2;

    //更新矩形：设置新大小并移回中心
    m_rect.setSize(QSize(newWidth, newHeight));
    m_rect.moveCenter(center);

    //同步缩放图片
    m_displayPixmap = m_originalPixmap.scaled(m_rect.size(), 
    Qt::IgnoreAspectRatio, 
    Qt::SmoothTransformation);
    update();
}

// 缩小 1.2 倍 (即除以 1.2)
void MainWindow::on_pushButton_3_clicked() {
    if (m_rect.isNull() || m_originalPixmap.isNull()) return;

    QPoint center = m_rect.center();

    // 防止矩形缩得太小消失（例如最小保持 10 像素）
    int newWidth = qMax(10, (int)(m_rect.width() / 1.2));
    int newHeight = qMax(10, (int)(m_rect.height() / 1.2));

    m_rect.setSize(QSize(newWidth, newHeight));
    m_rect.moveCenter(center);

    m_displayPixmap = m_originalPixmap.scaled(m_rect.size(), 
    Qt::IgnoreAspectRatio, 
    Qt::SmoothTransformation);
    update();
}