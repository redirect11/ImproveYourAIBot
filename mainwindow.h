#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QComboBox>
#include "webviewmanager.h"
#include "webserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onAppQuit();
    void on_actionLogout_triggered();
    void onPageLoaded(int page);
    void addChatbot();
    void deleteChatbot();
    void selectChatbot(const QString &name);

private slots:
    void on_actionNuovo_Chatbot_triggered();

    void on_actionRimuovi_Chatbot_triggered();


private:
    void changePage(const QString &page);
    void setupToolbarActions();
    void loadChatbots();

    Ui::MainWindow *ui;
    WebViewManager *webViewManager;
    WebServer *server;
    QComboBox *chatbotComboBox;
    QString currentChatbot;
    QMap<QString, QString> chatbots;
};

#endif // MAINWINDOW_H
