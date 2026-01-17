#include "MainWindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_startTime(QDateTime::currentDateTime())
    , m_timer(nullptr)
{
    setupUI();
    setWindowTitle("Qt Application");
    resize(400, 300);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    //显示 窗口启动后计时时间
    QLabel *startTimerLabel = new QLabel("Start Time:", centralWidget);
    QLineEdit *timeEdit = new QLineEdit(centralWidget);
    m_timeEdit = timeEdit;
    timeEdit->setReadOnly(true);
    QGridLayout *layout = new QGridLayout(centralWidget);
    layout->addWidget(startTimerLabel,0,1);
    layout->addWidget(timeEdit,0,2);
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::timeout);
    m_timer.start(10);
}
    
void MainWindow::timeout()
{
    //启动后的时间减去启动瞬间
    QDateTime currentTime = QDateTime::currentDateTime();
    qint64 elapsedSeconds = m_startTime.secsTo(currentTime);
    //显示在timeEdit中
    m_timeEdit->setText(QString::number(elapsedSeconds) + " 秒");
}
