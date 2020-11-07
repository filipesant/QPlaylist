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
#include "song.h"
#include "spotifycredentials.h"
#include "spotifydevice.h"
#include "spotifytimelapse.h"

class SpotifyRequests : public QObject
{
    Q_OBJECT

public:
    explicit SpotifyRequests(SpotifyCredentials *credentials,QObject *parent = nullptr);

    QJsonObject getAsObject(const QString &url);
    QJsonArray getAsArray(const QString &url);
    QVector<Song>  search(const QString &query);
    QVector<Song>  tracks(const QString &query);
    QByteArray getImage(const QString &url);

    QString addToQueue(const QString &uri);
    QVector<SpotifyDevice> devices();
    QString setDevice(const SpotifyDevice &device);
    QString playTracks(int trackIndex, const QString &context);
    QString playTracks(int trackIndex, const QStringList &all);
    QString playTracks(const QString &context);
    QString pause();
    QString resume();
    QString setVolume(int volume);
    QString seek(int position);
    bool isValid() const;

public slots:
    void requestCurrentPlayback();

private slots:
    void getLater(const QString &url);

signals:
    void got(const QJsonDocument &json);
    void gotPlayback(const SpotifyTimelapse &spotifyTimelapse);

private:
    qint64 lastAuth;
    QNetworkAccessManager *networkManager;
    bool refreshValid = false;
    QString accessToken;
    QString refreshToken;
    QString clientId;
    QString clientSecret;
    QString currentDevice;

    SpotifyCredentials *credentials;
    QNetworkRequest request(const QString &url);
    QJsonDocument get(const QString &url);

    QString put(const QString &url, QVariantMap *body = nullptr);
    QString post(const QString &url);
    QString del(const QString &url, const QJsonDocument &json);
    static QString errorMessage(QNetworkReply *reply);
    bool refresh();
};

#endif // SPOTIFYREQUESTS_H
