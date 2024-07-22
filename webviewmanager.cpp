#include "webviewmanager.h"
#include "optionsmanager.h"
#include <QDir>
#include <QFileInfo>
#include <QJSEngine>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringLiteral>
#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

WebViewManager::WebViewManager(QWidget *parent)
    : QObject(parent), webEngineView(new QWebEngineView(parent)) {

    qInfo() << QDir::currentPath() + "/assets/js/qwebchannel.js";
    QFileInfo jsFileInfo(QDir::currentPath() + "/assets/js/qwebchannel.js");

    if (!jsFileInfo.exists())
        QFile::copy(":/qtwebchannel/qwebchannel.js", jsFileInfo.absoluteFilePath());

    webEngineView->page()->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    webEngineView->page()->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    webEngineView->page()->settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);

    QWebChannel *channel = new QWebChannel(webEngineView->page());
    channel->registerObject(QStringLiteral("webViewManager"), this);
    webEngineView->page()->setWebChannel(channel);

    // Connect to loadFinished signal to inject the authToken
    connect(webEngineView->page(), &QWebEnginePage::loadFinished, this, &WebViewManager::injectAuthToken);
}

void WebViewManager::loadPage(const QString &pagePath) {
    webEngineView->load(QUrl("http://localhost:8000" + pagePath));
}

void WebViewManager::setAuthToken(const QString &token) {
    authToken = token;
    emit authTokenChanged(token);
    OptionsManager::instance().saveToken(baseUrl, token); // Save the token in options manager
}

void WebViewManager::setUserName(const QString &username)
{
    this->userName = username;
    emit userNameChanged(username);
    OptionsManager::instance().saveUserName(baseUrl, username);
}

void WebViewManager::logout()
{
    qInfo() << "Logging out";
    setAuthToken("");
    setUserName("");
    QString script = QStringLiteral("location.reload(true);");
    webEngineView->page()->runJavaScript(script);
}

void WebViewManager::setLocalUrl(const QString &url)
{
    qInfo() << url;
    QString script = QStringLiteral("window.setPageFromQt('%1');").arg(url);
    webEngineView->page()->runJavaScript(script);
}

void WebViewManager::hide()
{
    this->webEngineView->hide();
}

void WebViewManager::show()
{
    this->webEngineView->show();
}

void WebViewManager::handleAuthToken(const QString &token) {
    qInfo() << "Received auth token" << token;
    setAuthToken(token);
}

void WebViewManager::handleUserName(const QString &userName)
{
    qInfo() << "Received username" << userName;
    setUserName(userName);
}

void WebViewManager::injectAuthToken() {
    qInfo() << "Injecting auth token";
    QString script = QString("window.authToken = '%1';").arg(authToken);
    webEngineView->page()->runJavaScript(script);

    qInfo() << "Injecting base URL";
    QString baseUrlScript = QString("window.baseUrl = '%1';").arg(baseUrl);
    webEngineView->page()->runJavaScript(baseUrlScript);
}

QString WebViewManager::getAuthToken() const {
    qInfo() << "Getting auth token" << OptionsManager::instance().getToken(baseUrl);
    return OptionsManager::instance().getToken(baseUrl);
}

QString WebViewManager::getUserName() const
{
    qInfo() << "Getting username" << OptionsManager::instance().getUserName(baseUrl);
    return OptionsManager::instance().getUserName(baseUrl);
}

QString WebViewManager::getBaseUrl() const {
    return baseUrl;
}

QJSValue WebViewManager::getTrascriptionsIfAny() const
{
    // leggi la directory assets/trascrizioni
    QDir dir(QDir::currentPath() + "/assets/trascrizioni");
    QStringList files = dir.entryList(QDir::Files);

    //crea un QJsonArraya partire dal contenuto dei file .json
    QJsonArray transcriptions;
    for (const QString &file : files) {
        if (file.endsWith(".json")) {
            QFile jsonFile(dir.absoluteFilePath(file));
            if (jsonFile.open(QIODevice::ReadOnly)) {
                QJsonDocument doc = QJsonDocument::fromJson(jsonFile.readAll());
                transcriptions.append(doc.object());
            }
        }
    }

    qInfo() << transcriptions;

    QJSEngine engine;
    QJSValue jsTranscriptions = engine.toScriptValue(transcriptions);

    return jsTranscriptions;
}

void WebViewManager::setFileLoaded(QString file) {
    //cerca il file nella directory assets/trascrizioni e lo sposti nella directory "assets/trascrizioni/caricate"
    QDir dir(QDir::currentPath() + "/assets/trascrizioni");
    QDir dirLoaded(QDir::currentPath() + "/assets/trascrizioni/caricate");
    if (dirLoaded.exists() == false) {
        dirLoaded.mkpath(".");
    }
}

void WebViewManager::setBaseUrl(const QString &url) {
    baseUrl = url;
    authToken = OptionsManager::instance().getToken(url);
    userName = OptionsManager::instance().getUserName(url);
}

void WebViewManager::setCurrentChatbotName(const QString &name) {
    currentChatbotName = name;
}
