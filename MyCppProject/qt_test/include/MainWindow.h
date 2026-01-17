#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QPushButton;
class DrawingWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCreateCircle();
    void onCreateSquare();
    void onCreateTriangle();

private:
    void setupUI();
    
    DrawingWidget *m_drawingWidget;
    QPushButton *m_btnCircle;
    QPushButton *m_btnCircle2;
    QPushButton *m_btnCircle3;
};
