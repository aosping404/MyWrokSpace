#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("qttest");
    app.setOrganizationName("MyCompany");
    app.setOrganizationDomain("example.com");

    // 加载翻译文件
    // Qt 6 的 qt_add_translations 会自动处理翻译文件部署
    // 翻译文件会从应用程序目录或标准位置自动加载
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qttest_" + QLocale(locale).name();
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

    MainWindow window;
    window.show();

    return app.exec();
}
