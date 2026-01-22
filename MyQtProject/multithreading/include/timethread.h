#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QThread>
#include <QWidget>
class MainWindow;
class TimeThread:public QThread
{
public:
    explicit TimeThread(QObject *parent = nullptr);
    void stop();
    MainWindow * m_mainWindow;
    QWidget * m_mainWindow;
protected:
    void run() override;
private:
    bool m_stoped=false;
};

#endif //TIMETHREAD_H