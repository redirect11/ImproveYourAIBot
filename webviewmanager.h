#ifndef WEBVIEWMANAGER_H
#define WEBVIEWMANAGER_H

#include <QJSValue>
#include <QObject>
#include <QWebEngineView>
#include <QWindow>


class CustomWebEnginePage : public QWebEnginePage {
    Q_OBJECT
public:
    CustomWebEnginePage(QObject *parent = nullptr) : QWebEnginePage(parent) {}

// protected:
//     QWebEnginePage* createWindow(QWebEnginePage::WebWindowType type) override {
//         Q_UNUSED(type);
//         qInfo() << "Creating a new window";

//         // QWebEngineView *newView = new QWebEngineView();

//         // //aggiungi newView in un layout e aggiungi il layout in wdg
//         // QWindow *newWindow = new QWindow();
//         // newWindow->setGeometry(100, 100, 800, 600);
//         // newWindow->setTitle("Nuova Finestra");
//         // newWindow->setFlags(Qt::Window);

//         // // Crea una nuova pagina personalizzata
//         // CustomWebEnginePage *newPage = new CustomWebEnginePage(newView);

//         // // Imposta la nuova pagina nella QWebEngineView
//         // newView->setPage(newPage);

//         //  // Imposta l'URL che vuoi caricare nella nuova finestra
//         // newView->load(QUrl("http://example.com"));  // Modifica con l'URL desiderato

//         // // Mostra la finestra
//         // newWindow->show();

//         return newPage;
//     }
};

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
    CustomWebEnginePage* page;
};

#endif // WEBVIEWMANAGER_H
