#include "MainWindow.h"
#include "CubeWidget.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建主容器
    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);
    
    // 创建立方体组件
    m_cubeWidget = new CubeWidget();
    layout->addWidget(m_cubeWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    
    setCentralWidget(container);
    setWindowTitle("Rotating Cube - Magic Number 0x5f3759df");
    resize(700, 700);
}

MainWindow::~MainWindow()
{
}
