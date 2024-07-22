#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>
#include <QThread>
#include "httplib.h"

#define CHATBOT_SETTINGS_PAGE "/chatbot-settings"
#define ASSISTANTS_PAGE "/assistants-settings"
#define TRANSCRIPTIONS_PAGE "/transcriptions-settings"

class WebServer : public QObject {
    Q_OBJECT

public:
    WebServer(const char* host, int port, const char* web_root, QObject *parent = nullptr);
    ~WebServer();

public slots:
    void start();
    void stop();
    void run();

signals:
    void serverStarted();
    void serverStopped();

private:
    const char* host_;
    int port_;
    const char* web_root_;
    bool running_;
    QThread *thread_;
    httplib::Server server_;
};

#endif // WEBSERVER_H
