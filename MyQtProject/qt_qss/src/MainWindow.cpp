#include "ui_MainWindow.h" // 引入自动生成的 UI 头文件（必须在 MainWindow.h 之前）
#include "MainWindow.h"
#include "IkeaShowcaseWindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QComboBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 初始化 UI 指针
{
    ui->setupUi(this); // 加载 UI 布局
    
    // 初始化样式下拉框
    initializeStyleComboBox();
    
    // 连接信号槽
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(ui->styleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStyleChanged);
    connect(ui->openIkeaShowcaseButton, &QPushButton::clicked, 
            this, &MainWindow::onOpenIkeaShowcase);
    
    setWindowTitle("Qt QSS 样式展示");
    
    // 加载默认样式（ElegantDark）
    loadStyleSheet("ElegantDark");
    // 设置下拉框默认选中 ElegantDark
    int defaultIndex = ui->styleComboBox->findText("ElegantDark");
    if (defaultIndex >= 0) {
        ui->styleComboBox->setCurrentIndex(defaultIndex);
    }
}

MainWindow::~MainWindow()
{
    delete ui; // 释放 UI 指针
}

void MainWindow::onButtonClicked()
{
    QMessageBox::information(this, "提示", "按钮被点击了！\n这是一个 Qt 应用程序示例。");
}

void MainWindow::initializeStyleComboBox()
{
    // 添加所有可用的样式主题到下拉框
    QStringList styles = {
        "IKEA",             // IKEA 宜家风格主题
        "ElegantDark",      // 优雅深色主题
        "MaterialDark",     // Material Design 深色主题
        "AMOLED",           // AMOLED 黑色主题
        "QDarkStyleSheet",  // Qt 深色样式表
        "ConsoleStyle",     // 控制台风格主题
        "Ubuntu",           // Ubuntu 风格主题
        "Aqua",             // Aqua 风格主题
        "MacOS",            // macOS 风格主题
        "white",            // 白色主题
        "ManjaroMix",       // Manjaro 混合主题
        "NeonButtons",      // 霓虹按钮主题
        "coffee",           // 咖啡色主题
        "khaki",            // 卡其色主题
        "blue",             // 蓝色主题
        "transparent",      // 透明主题
        "pagefold",         // 页面折叠风格
        "phonegui",         // 手机 GUI 风格
        "style",            // 风格主题
        "style2",           // 风格主题 2
        "style3",           // 风格主题 3
        "style4",           // 风格主题 4
        "stylesheet2",      // 样式表 2
        "or",               // OR 主题
        "abc",              // ABC 主题
        "abc_satellite",    // ABC 卫星主题
        "application",      // 应用样式
        "default",          // 默认样式
        "default2",         // 默认样式 2
        "OverlayWidget",   // 覆盖小部件样式
        "nostyle",          // 无样式
        "no"                // 无样式
    };
    
    ui->styleComboBox->addItems(styles);
}

void MainWindow::onStyleChanged(int index)
{
    if (index < 0) return;
    
    QString styleName = ui->styleComboBox->itemText(index);
    loadStyleSheet(styleName);
}

void MainWindow::loadStyleSheet(const QString &styleName)
{
    // 从资源文件加载 QSS 样式
    QString stylePath = QString(":/styles/%1.qss").arg(styleName);
    QFile file(stylePath);
    
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        qApp->setStyleSheet(styleSheet);
        file.close();
    } else {
        // 如果资源文件加载失败，尝试从文件系统加载
        QString filePath = QString("Styles/%1.qss").arg(styleName);
        QFile fsFile(filePath);
        if (fsFile.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&fsFile);
            QString styleSheet = stream.readAll();
            qApp->setStyleSheet(styleSheet);
            fsFile.close();
        } else {
            QMessageBox::warning(this, "样式加载失败", 
                QString("无法加载样式文件: %1\n请确保样式文件存在于资源或文件系统中。").arg(styleName));
        }
    }
}

void MainWindow::onOpenIkeaShowcase()
{
    // 创建并显示 IKEA 展示窗口
    IkeaShowcaseWindow* showcaseWindow = new IkeaShowcaseWindow(this);
    showcaseWindow->setAttribute(Qt::WA_DeleteOnClose);
    showcaseWindow->show();
}
