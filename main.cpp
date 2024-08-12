#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[])
{
    qInfo() << "Starting application";
    qInfo() << "Arguments: " << argc;
    char ARG_DISABLE_WEB_SECURITY[] = "--disable-web-security";
    char ARG_ENABLE_FILE_ACCESS[] = "--enable-file-access-from-files";
    char ARG_ALLOW_CORS[] = "--webEngineArgs";
    char ARG_ALLOW_CORS_ARGS[] = "--remote-allow-origins=*";
    int newArgc = argc+1+1+1+1+1;
    char** newArgv = new char*[newArgc];
    for(int i=0; i<argc; i++) {
        newArgv[i] = argv[i];
    }
    newArgv[argc] = ARG_ALLOW_CORS;
    newArgv[argc+1] = ARG_ALLOW_CORS_ARGS;
    newArgv[argc+2] = ARG_DISABLE_WEB_SECURITY;
    newArgv[argc+3] = ARG_ENABLE_FILE_ACCESS;
    newArgv[argc+4] = nullptr;

    QApplication a(newArgc, newArgv);
        qInfo()  << "running path" << QCoreApplication::applicationDirPath();
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ImproveYourAIBot_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    //print arguments
    for(int i=0; i<newArgc; i++) {
        qInfo() << "Argument " << i << ": " << newArgv[i];
    }

    MainWindow w;
    w.setWindowTitle("ImproveYourAIBot - v1.1 - CPU");
    //w.setWindowIcon(QIcon(":/icons/favicon.ico"));
    a.setWindowIcon(QIcon(":/icons/favicon.ico"));
    QObject::connect(&a, &QApplication::aboutToQuit, &w, &MainWindow::onAppQuit);
    w.show();


    return a.exec();
}
