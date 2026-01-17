#pragma once

#include <QMainWindow>

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
    void onButtonClicked();

private:
    void setupUI();
    
    QPushButton *m_button;
    QPushButton *m_button2;
    QPushButton *m_button3;
    QPushButton *m_button4;
    QPushButton *myPngButton;
};
