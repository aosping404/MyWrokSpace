#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class DialogStudentInput; } // 必须与 .ui 文件中的类名一致
QT_END_NAMESPACE

struct Student{
    int index;
    QString name;
    int age;
    double math;
    double chinese;
    double english;
};

class DialogStudentInput : public QDialog{
    Q_OBJECT
public:
    explicit DialogStudentInput(QWidget *parent = nullptr);
    ~DialogStudentInput();
    void setData(Student &stu);
    void getData(Student &stu);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
private:
    Ui::DialogStudentInput *ui;
};
