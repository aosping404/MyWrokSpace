#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

//取号双线程异步
void MainWindow::on_pushButton_clicked() {
    // 每次取号前清空旧数据
    m_list1.clear();
    m_list2.clear();
    ui->textEdit->clear();
    ui->textEdit_2->clear();

    // 1. 创建线程1 和 工人1
    QThread *thread1 = new QThread();
    Worker *worker1 = new Worker();
    worker1->moveToThread(thread1);

    connect(thread1, &QThread::started, worker1, &Worker::doWork);
    connect(worker1, &Worker::workFinished, this, [this](const QList<int> &data) {
        m_list1 = data; // 存入内存数组
        QStringList strList;
        for(int n : data) strList << QString::number(n);
        ui->textEdit->setPlainText(strList.join(',')); // 显示到界面
    });

    // 资源自动释放
    connect(worker1, &Worker::workFinished, thread1, &QThread::quit);
    connect(worker1, &Worker::workFinished, worker1, &QObject::deleteLater);
    connect(thread1, &QThread::finished, thread1, &QObject::deleteLater);
    thread1->start();

    // 2. 创建线程2 和 工人2
    QThread *thread2 = new QThread();
    Worker *worker2 = new Worker();
    worker2->moveToThread(thread2);

    connect(thread2, &QThread::started, worker2, &Worker::doWork);
    connect(worker2, &Worker::workFinished, this, [this](const QList<int> &data) {
        m_list2 = data; // 存入内存数组
        QStringList strList;
        for(int n : data) strList << QString::number(n);
        ui->textEdit_2->setPlainText(strList.join(',')); // 显示到界面
    });

    // 资源自动释放
    connect(worker2, &Worker::workFinished, thread2, &QThread::quit);
    connect(worker2, &Worker::workFinished, worker2, &QObject::deleteLater);
    connect(thread2, &QThread::finished, thread2, &QObject::deleteLater);
    thread2->start();
}

void MainWindow::on_pushButton_2_clicked() {
    if (m_list1.isEmpty() || m_list2.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先点击取号生成数据！");
        return;
    }

    // 利用 QSet 快速查找交集
    QSet<int> set1(m_list1.begin(), m_list1.end());
    QSet<int> commonSet;

    for (int num : m_list2) {
        if (set1.contains(num)) {
            commonSet.insert(num);
        }
    }

    // 结果反馈
    if (commonSet.isEmpty()) {
        QMessageBox::information(this, "结果", "未发现重复号码。");
    } else {
        QStringList resStrings;
        // 将数字转回字符串展示
        for (int n : commonSet) resStrings << QString::number(n);
        QMessageBox::information(this, "结果", "发现同号数字：\n" + resStrings.join(", "));
    }
}