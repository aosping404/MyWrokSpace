#include "MainWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QDialog>
#include <QLineEdit>
#include <QApplication>
#include <QMouseEvent>
#include <QLine>
#include <QVector>
#include <QPainter>
#include "scribblewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new scribbleWidget(this));
    setWindowTitle("Qt Application");
    resize(400, 300);
}

MainWindow::~MainWindow()
{
    
}

