#include "MainWindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QSlider>
#include <QMessageBox>

//构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_button(nullptr)
    , m_button2(nullptr)
    , m_button3(nullptr)
    , m_button4(nullptr)
    , myPngButton(nullptr)
{
    setupUI();
    setWindowTitle("Qt Video Player");
    resize(800, 600);
}

//析构函数
MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // --- 上部区域：视频 + 列表 ---
    QHBoxLayout *topLayout = new QHBoxLayout();
    
    QLabel *videoDisplay = new QLabel("视频播放区域", this);
    videoDisplay->setObjectName("videoDisplay");
    videoDisplay->setAlignment(Qt::AlignCenter);
    
    QFrame *sideBar = new QFrame(this);
    sideBar->setObjectName("sideBar");
    sideBar->setFixedWidth(150);
    QVBoxLayout *sideLayout = new QVBoxLayout(sideBar);
    sideLayout->addWidget(new QLabel("播放列表", this));
    sideLayout->addStretch();

    topLayout->addWidget(videoDisplay, 1);
    topLayout->addWidget(sideBar);
    mainLayout->addLayout(topLayout);

    // --- 下部区域：控制栏 ---
    QFrame *controlBar = new QFrame(this);
    controlBar->setObjectName("controlBar");
    controlBar->setFixedHeight(80);
    
    QVBoxLayout *controlLayout = new QVBoxLayout(controlBar);
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    controlLayout->addWidget(slider);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    m_button2 = new QPushButton("播放", this);
    m_button3 = new QPushButton("停止", this);
    myPngButton = new QPushButton("全屏", this);
    
    m_button2->setObjectName("m_button2");
    m_button3->setObjectName("m_button3");
    myPngButton->setObjectName("myPngButton");

    buttonsLayout->addWidget(m_button2);
    buttonsLayout->addWidget(m_button3);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(myPngButton);
    
    controlLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(controlBar);

    // 连接信号
    connect(m_button2, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(m_button3, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(myPngButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
}

// 4. 报错中提到的 onButtonClicked 必须实现
void MainWindow::onButtonClicked()
{
    QMessageBox::information(this, "提示", "功能开发中...");
}