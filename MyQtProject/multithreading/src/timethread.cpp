#include "timethread.h"
#include "MainWindow.h"
TimeThread::TimeThread(QObject *parent)
    : QThread{parent}
{}

void TimeThread::stop()
{
    m_stoped=true;
    wait();
}
void TimeThread::run()
{
    m_stoped=false;
    while(!m_stoped)
    {
        m_mainWindow->update();
        msleep(100);
    }
}