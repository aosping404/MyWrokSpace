#pragma once

#include <QThread>
#include <QPushButton>

class CountThread : public QThread
{
    Q_OBJECT

public:
    explicit CountThread(QPushButton *button, QObject *parent = nullptr);
    ~CountThread();

signals:
    void countUpdated(const QString &text);  // 发送计数更新信号

protected:
    void run() override;

private:
    QPushButton *m_button;  // 要更新的按钮指针
    int m_count;            // 计数器
};
