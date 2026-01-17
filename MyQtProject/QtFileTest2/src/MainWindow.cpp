#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QSettings>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 确定可读写的路径：通常放在程序运行目录下
    m_iniPath = QCoreApplication::applicationDirPath() + "/config.ini";

    // 初始化文件：如果本地没有，从资源文件复制一个出来
    initIniFile();

    // 一次性读取加载到界面
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initIniFile()
{
    if (!QFile::exists(m_iniPath)) {
        // 如果磁盘没有 config.ini，从 qrc 资源复制一个初始模板出来
        QFile::copy(":/ini/setup.ini", m_iniPath);
        // 去除只读属性
        QFile::setPermissions(m_iniPath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
}

// --- 快速一次性读取 ---
void MainWindow::loadSettings()
{
    QSettings settings(m_iniPath, QSettings::IniFormat);

    settings.beginGroup("General");
    
    // 使用 value(key, defaultValue) 形式，防止文件缺失导致报错
    ui->checkBox->setChecked(settings.value("basketball", "否").toString() == "是");
    ui->checkBox_2->setChecked(settings.value("climbMountains", "否").toString() == "是");
    ui->checkBox_3->setChecked(settings.value("readBook", "否").toString() == "是");
    ui->checkBox_4->setChecked(settings.value("soccer", "否").toString() == "是");
    QString sex = settings.value("sex", "男").toString();
    if (sex == "女") {
        ui->radioButton->setChecked(true);
    } else {
        ui->radioButton_2->setChecked(true);
    }
    
    settings.endGroup();
    qDebug() << "Settings Loaded from:" << m_iniPath;
}

// --- 快速一次性保存 ---
void MainWindow::saveSettings()
{
    QSettings settings(m_iniPath, QSettings::IniFormat);

    settings.beginGroup("General");
    
    settings.setValue("basketball", ui->checkBox->isChecked() ? "是" : "否");
    settings.setValue("climbMountains", ui->checkBox_2->isChecked() ? "是" : "否");
    settings.setValue("readBook", ui->checkBox_3->isChecked() ? "是" : "否");
    settings.setValue("soccer", ui->checkBox_4->isChecked() ? "是" : "否");
    settings.setValue("sex", ui->radioButton->isChecked() ? "女" : "男");
    
    settings.endGroup();
    settings.sync(); // 强制写入磁盘
    qDebug() << "Settings Saved to:" << m_iniPath;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}