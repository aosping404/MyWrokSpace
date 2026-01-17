#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void timeout();
private:
    void setupUI();
private:
    QDateTime m_startTime; // 记录启动瞬间
    QLineEdit *m_timeEdit;
    QTimer m_timer;
};
