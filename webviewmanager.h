#ifndef WEBVIEWMANAGER_H
#define WEBVIEWMANAGER_H

#include <QJSValue>
#include <QObject>
#include <QWebEngineView>

class WebViewManager : public QObject
{
    Q_OBJECT

public:
    explicit WebViewManager(QWidget *parent = nullptr);

    QWebEngineView* getWebEngineView() const { return webEngineView; }
    Q_INVOKABLE  QString getAuthToken() const;
    Q_INVOKABLE  QString getUserName() const;
    Q_INVOKABLE  QString getBaseUrl() const;
    Q_INVOKABLE QJSValue getTrascriptionsIfAny() const;
    Q_INVOKABLE void setFileLoaded(QString file);
    void setAuthToken(const QString &token);
    void setUserName(const QString &username);
    void setBaseUrl(const QString &url);
    void setCurrentChatbotName(const QString &name);
    void loadPage(const QString &pagePath);
    void hide();
    void show();

public slots:
    void handleAuthToken(const QString &token);
    void handleUserName(const QString &userName);
    void logout();
    void setLocalUrl(const QString &url);


signals:
    void authTokenChanged(const QString &token);
    void userNameChanged(const QString &userName);
    void baseUrlChanged(const QString &baseUrl);

private:
    void injectAuthToken();

    QWebEngineView *webEngineView;
    QString authToken;
    QString userName;
    QString baseUrl;
    QString currentChatbotName;
};

#endif // WEBVIEWMANAGER_H
