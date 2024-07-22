#ifndef OPTIONSMANAGER_H
#define OPTIONSMANAGER_H

#include <QString>
#include <QSettings>
#include <QMap>

class OptionsManager
{
public:
    static OptionsManager& instance()
    {
        static OptionsManager instance;
        return instance;
    }

    QString getToken(const QString& baseUrl) const
    {
        return getSetting(baseUrl + "_" + AUTH_TOKEN_KEY, "");
    }

    void saveToken(const QString &baseUrl, const QString &token)
    {
        saveSetting(baseUrl + "_" + AUTH_TOKEN_KEY, token);
    }

    QString getUserName(const QString& baseUrl) const
    {
        return getSetting(baseUrl + "_" + USERNAME_KEY, "");
    }

    void saveUserName(const QString &baseUrl, const QString &username)
    {
        saveSetting(baseUrl + "_" + USERNAME_KEY, username);
    }

    QMap<QString, QString> getChatbots() const
    {
        QMap<QString, QString> chatbots;
        QSettings settings(SCOPE, COMPANY_NAME, APP_NAME);
        qInfo() << "Reading chatbots" << settings.fileName();
        qInfo() << "All keys" << settings.allKeys();
        int size = settings.beginReadArray("Chatbots");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            QString url(settings.value("url").toString());
            QString name(settings.value("name").toString());
            chatbots[url] = name;
        }
        settings.endArray();
        qInfo() << "Chatbots" << chatbots;
        return chatbots;
    }

    void saveChatbots(const QMap<QString, QString> &chatbots)
    {
        QSettings settings(SCOPE, COMPANY_NAME, APP_NAME);
        settings.beginWriteArray("Chatbots");
        int index = 0;
        for (const QString &url : chatbots.keys()) {
            qInfo() << "Saving chatbot" << url << chatbots[url];
            settings.setArrayIndex(index);
            settings.setValue("url", url);
            settings.setValue("name", chatbots[url]);
            index++;
        }
        settings.endArray();
        settings.sync();
    }


private:
    OptionsManager() {}
    OptionsManager(const OptionsManager&) = delete;
    OptionsManager& operator=(const OptionsManager&) = delete;

    QString getSetting(const QString &key, const QString &defaultValue) const
    {
        QSettings settings(SCOPE, COMPANY_NAME, APP_NAME);
        return settings.value(key, defaultValue).toString();
    }

    void saveSetting(const QString &key, const QString &value)
    {
        QSettings settings(SCOPE, COMPANY_NAME, APP_NAME);
        settings.setValue(key, value);
    }

    static constexpr const char* COMPANY_NAME = "AEMME";
    static constexpr const char* APP_NAME = "ImproveYoutAiBot";
    static constexpr const char* AUTH_TOKEN_KEY = "authToken";
    static constexpr const char* USERNAME_KEY = "username";
    static constexpr const QSettings::Scope SCOPE = QSettings::UserScope;
};

#endif // OPTIONSMANAGER_H
