#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class CubeWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    CubeWidget *m_cubeWidget;
};
