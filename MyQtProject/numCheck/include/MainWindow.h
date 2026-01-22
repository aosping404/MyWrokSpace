#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QRandomGenerator>
#include <QList>
#include <QSet>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//工人类-后台生成数字
class Worker : public QObject {
    Q_OBJECT
public slots:
    void doWork() {
        QList<int> numbers;
        for (int i = 0; i < 50; ++i) {
            //1-100的随机数
            numbers << QRandomGenerator::global()->bounded(1, 101);
        }
        emit workFinished(numbers); // 发送整型列表
    }
signals:
    void workFinished(const QList<int> &data);
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked(); 

private:
    Ui::MainWindow *ui;
    QList<int> m_list1;
    QList<int> m_list2;
};

#endif // MAINWINDOW_H