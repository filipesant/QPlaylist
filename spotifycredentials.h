#ifndef SPOTIFYCREDENTIALS_H
#define SPOTIFYCREDENTIALS_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTcpServer>
#include <QUrl>
#include <QFileInfo>
#include <QMutex>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>


#include <QString>
#include <QUrl>

class SpotifyCredentials :public QObject
{
    Q_OBJECT

public:
    explicit SpotifyCredentials();
    virtual ~SpotifyCredentials();
    static QString authUrl(const QString &clientId, const QString &redirect);
    QString auth(const QString &code, const QString &redirect, const QString &id, const QString &secret);

    QString clientId();
    QString clientSecret();
    QString redirectUri();
    QString accessToken();
    QString refreshToken();

    static QString fileName();
    static QString filePath();

    QJsonObject toJson();
    void fromJson(const QJsonObject &json);
    void save();
    void load();

private:
    QNetworkAccessManager *networkManager;
    QMutex mutex;
    QTcpServer *server;
    QString m_clientIdText, m_clientSecretText, m_redirect, m_accessToken, m_refreshToken;

    template<typename T>
    void setValue(const QJsonObject &json, const QString &key, T &value);


};

#endif // SPOTIFYCREDENTIALS_H
