#include "countThread.h"

CountThread::CountThread(QPushButton *button, QObject *parent)
    : QThread(parent)
    , m_button(button)
    , m_count(0)
{
}

CountThread::~CountThread()
{
    // 请求线程退出
    requestInterruption();
    // 等待线程结束
    wait();
}

void CountThread::run()
{
    while (!isInterruptionRequested()) {
        m_count++;
        // 使用信号槽机制确保线程安全地更新UI
        emit countUpdated(QString::number(m_count));
        msleep(100);  // 休眠100毫秒
    }
}