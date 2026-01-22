#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "MainWindow.h"
#include "logingPage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("keypress");
    app.setOrganizationName("MyCompany");
    app.setOrganizationDomain("example.com");

    // 加载翻译文件
    // Qt 6 的 qt_add_translations 会自动处理翻译文件部署
    // 翻译文件会从应用程序目录或标准位置自动加载
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "keypress_" + QLocale(locale).name();
        // 首先尝试从应用程序目录加载（Qt 6 自动部署的位置）
        if (translator.load(baseName, app.applicationDirPath())) {
            app.installTranslator(&translator);
            break;
        }
        // 如果未找到，尝试从资源文件加载（如果手动添加到资源文件）
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    logingPage loginDialog;
    
    // 显示模态登录窗口，如果用户点击"取消"或关闭窗口，返回 Rejected
    if(loginDialog.exec() == QDialog::Accepted) {
        // 登录成功，显示主窗口
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    }
    
    // 登录失败或取消，退出程序
    return 0;
}
