#include "MainWindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QDialog>  // 添加此头文件用于QDialog

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , userName(nullptr)
    , userPassword(nullptr) // --- IGNORE ---
    , m_loginButton(nullptr)
    , m_canselButton(nullptr)
    , m_userNameEdit(nullptr)
    , m_passwordEdit(nullptr)
    , m_modalButton(nullptr)  // 添加成员变量
    , m_nonModalButton(nullptr)  // 添加成员变量
    , m_moveButton(nullptr) // --- IGNORE ---
{
    setupUI();
    setWindowTitle("Shigure");
    resize(400, 300);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    //用户登陆窗口
    //用户名输入框
    //密码输入框

    userName = new QLabel("用户：", this);
    userPassword = new QLabel("密码：", this); // --- IGNORE ---
    m_loginButton = new QPushButton("登陆", this);
    m_userNameEdit = new QLineEdit(this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_canselButton = new QPushButton("取消", this);

    // 添加模态和非模态窗口按钮
    m_modalButton = new QPushButton("打开模态窗口", this);
    m_nonModalButton = new QPushButton("打开非模态窗口", this);
    m_moveButton = new QPushButton("移动窗口", this); // --- IGNORE ---
    layout->addWidget(userName);
    layout->addWidget(m_userNameEdit);
    layout->addWidget(userPassword);
    layout->addWidget(m_passwordEdit);
    layout->addWidget(m_loginButton);
    layout->addWidget(m_canselButton);
    layout->addWidget(m_modalButton);  // 添加到布局
    layout->addWidget(m_nonModalButton);  // 添加到布局
    //输入框
    layout->addWidget(m_moveButton); // --- IGNORE ---

    connect(m_moveButton, &QPushButton::clicked, [this]() {
        this->move(this->x() + 100, this->y() + 100);
    }); // --- IGNORE ---
    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
    connect(m_canselButton, &QPushButton::clicked, this, &MainWindow::onCanselButtonClicked);
    connect(m_modalButton, &QPushButton::clicked, this, &MainWindow::onModalButtonClicked);  // 连接信号
    connect(m_nonModalButton, &QPushButton::clicked, this, &MainWindow::onNonModalButtonClicked);  // 连接信号
}

void MainWindow::onLoginButtonClicked()
{
    QMessageBox::information(this, "提示", "登陆成功");

}
void MainWindow::onCanselButtonClicked()
{
    close();
}

// 添加槽函数
void MainWindow::onModalButtonClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("模态窗口");
    dialog.setModal(true);  // 设置为模态
    dialog.exec();  // 阻塞执行
}

void MainWindow::onNonModalButtonClicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("非模态窗口");
    dialog->setModal(false);  // 设置为非模态（默认）
    dialog->show();  // 非阻塞显示
}
