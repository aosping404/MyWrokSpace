#include "MainWindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_userName(nullptr)
    , m_userPassword(nullptr)
    , m_login(nullptr)
    , m_userNameLineEdit(nullptr)
    , m_userPasswordLineEdit(nullptr)
    , m_mainInterface(nullptr)  // 添加主界面窗口指针
{
    setupUI();
    setWindowTitle("Qt Application");
    resize(500, 200);
}

MainWindow::~MainWindow()
{
    if (m_mainInterface) {
        delete m_mainInterface;
    }
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    //添加菜单栏
    QMenuBar *menuBar = this->menuBar();
    QMenu *settingsMenu = menuBar->addMenu("设置");
    //将”分辨率设置“添加到设置菜单中
    QMenu *resolutionMenu = settingsMenu->addMenu("分辨率设置");
    QAction *action1080p = resolutionMenu->addAction("1920x1080");
    QAction *action720p = resolutionMenu->addAction("1280x720");
    connect(action1080p, &QAction::triggered, this, &MainWindow::on1080pClicked);
    connect(action720p, &QAction::triggered, this, &MainWindow::on720pClicked);


    m_userName = new QLabel("用户名：", this);
    m_userName->setAlignment(Qt::AlignCenter);
    m_userName->setStyleSheet("font-size: 18px; font-weight: bold;");

    m_userPassword = new QLabel("用户密码：", this);
    m_userPassword->setAlignment(Qt::AlignCenter);
    m_userPassword->setStyleSheet("font-size: 18px; font-weight: bold;");

    m_login = new QPushButton("登陆", this);
    m_login->setStyleSheet("font-size: 14px; padding: 10px;");

    m_userNameLineEdit = new QLineEdit(this);
    m_userPasswordLineEdit = new QLineEdit(this);
    m_userPasswordLineEdit->setEchoMode(QLineEdit::Password);

    layout->addWidget(m_userName);
    layout->addWidget(m_userNameLineEdit);  
    layout->addWidget(m_userPassword);
    layout->addWidget(m_userPasswordLineEdit);
    layout->addWidget(m_login);
    connect(m_login, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);

}

void MainWindow::onLoginButtonClicked()
{
    if(m_userNameLineEdit->text().isEmpty() || m_userPasswordLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名或密码不能为空！");
        return;
    }
    if(m_userNameLineEdit->text() != "a" || m_userPasswordLineEdit->text() != "p") {
        QMessageBox::critical(this, "错误", "用户名或密码错误！");
        return;
    }
    //继续打开新窗口，绘制主界面
    if (!m_mainInterface) {
        m_mainInterface = new MainInterface();
    }
    m_mainInterface->show();
    this->hide();  // 隐藏登录窗口
}

void MainWindow::onBackToLogin()
{
    if (m_mainInterface) {
        m_mainInterface->hide();
    }
    this->show();  // 显示登录窗口
}

void MainWindow::onSettingsButtonClicked()
{
    QMessageBox::information(this, "设置", "打开设置窗口！");
}
void MainWindow::onWindowSizeClicked()
{

}
void MainWindow::on1080pClicked()
{
    resize(1920, 1080);
}
void MainWindow::on720pClicked()
{
    resize(500, 200);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::information(this, "提示", "确定要退出应用程序？",QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

MainInterface::MainInterface(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("主界面");
    resize(600, 600);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QPushButton *backButton = new QPushButton("返回登录界面", this);
    mainLayout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, [this]() {
        this->hide();
        emit backToLogin();
    });
}

void MainInterface::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawLine(100, 100, 400, 400);
}
void MainInterface::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::information(this, "提示", "确定要退出应用程序？",QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}