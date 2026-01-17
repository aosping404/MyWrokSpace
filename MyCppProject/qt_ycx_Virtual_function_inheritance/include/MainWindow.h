#pragma once

#include <QMainWindow>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginButtonClicked(   );
    void onCanselButtonClicked(   );
    void onModalButtonClicked();  // 添加槽函数
    void onNonModalButtonClicked();  // 添加槽函数

private:
    void setupUI();
    
    QLabel *userName;
    QLabel *userPassword; // --- IGNORE ---
    QPushButton *m_loginButton; // --- IGNORE ---
    QPushButton *m_canselButton; // --- IGNORE ---
    QLineEdit *m_userNameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_modalButton;  // 添加成员变量
    QPushButton *m_nonModalButton;  // 添加成员变量

    //窗口移动按钮
    QPushButton *m_moveButton; // --- IGNORE ---
};
