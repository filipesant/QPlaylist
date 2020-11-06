#ifndef SPOTIFYREQUESTS_H
#define SPOTIFYREQUESTS_H

#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QSettings>
#include <QString>
#include <QtNetwork>

class SpotifyRequests : public QObject
{
    Q_OBJECT

public:
    explicit SpotifyRequests(QObject *parent = nullptr);

    QJsonObject getAsObject(const QString &url);
    QJsonArray getAsArray(const QString &url);

    QString pause();
    QString resume();
    QString seek(int position);
    bool isValid() const;

signals:
    void got(const QJsonDocument &json);

private:
    qint64 lastAuth;
    QNetworkAccessManager *networkManager;
    bool refreshValid = false;
    QString accessToken;
    QString refreshToken;
    QString clientId;
    QString clientSecret;

    QNetworkRequest request(const QString &url);
    QJsonDocument get(const QString &url);
    QString put(const QString &url, QVariantMap *body = nullptr);
    QString post(const QString &url);
    QString del(const QString &url, const QJsonDocument &json);
    static QString errorMessage(QNetworkReply *reply);
    bool refresh();
};

#endif // SPOTIFYREQUESTS_H
