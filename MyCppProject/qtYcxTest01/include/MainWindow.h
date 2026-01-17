#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginButtonClicked();
    void onSettingsButtonClicked();
    void onWindowSizeClicked();
    void on1080pClicked();
    void on720pClicked();
    void onBackToLogin();

private:
    void setupUI();
    
    QLabel *m_userName;
    QLabel *m_userPassword;
    QPushButton *m_login;
    QLineEdit *m_userNameLineEdit;
    QLineEdit *m_userPasswordLineEdit;
    QWidget *m_mainInterface;  // 主界面窗口指针
protected:
    void closeEvent(QCloseEvent *event) override;
};
class  MainInterface: public QWidget
{
    Q_OBJECT
public:
    MainInterface(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
signals:
    void backToLogin();
};