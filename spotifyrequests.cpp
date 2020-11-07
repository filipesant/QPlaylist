#include "spotifyrequests.h"

SpotifyRequests::SpotifyRequests(SpotifyCredentials *credentials, QObject *parent) :
    QObject(parent), credentials(credentials)
{
    lastAuth = 0;
    networkManager = new QNetworkAccessManager(this);
    refreshValid = refresh();
}

QNetworkRequest SpotifyRequests::request(const QString &url)
{
    // See when last refresh was
    auto lastRefresh = QDateTime::currentSecsSinceEpoch() - lastAuth;
    if (lastRefresh >= 3600)
    {
        qDebug() << "Access token probably expired, refreshing";
        refresh();
    }

    // Prepare request
    QNetworkRequest request((QUrl("https://api.spotify.com/v1/" + url)));

    // Set header
    request.setRawHeader("Authorization",
                         ("Bearer " + accessToken).toUtf8());

    // Return prepared header
    return request;
}

QString SpotifyRequests::put(const QString &url, QVariantMap *body)
{
    // Set in header we're sending json data
    auto req = request(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    // Send the request, we don't expect any response
    auto putData = body == nullptr ? nullptr : QJsonDocument::fromVariant(*body).toJson();
    auto reply = errorMessage(networkManager->put(req, putData));
    if (reply.contains("No active device found"))
    {
        auto d = devices();
        if (d.length() == 1)
        {
            setDevice(devices().at(0));
            return put(url, body);
        }
    }
    return reply;
}

QString SpotifyRequests::post(const QString &url)
{
    auto req = request(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    return errorMessage(networkManager->post(req, QByteArray()));
}

QString SpotifyRequests::del(const QString &url, const QJsonDocument &json)
{
    auto req = request(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return errorMessage(networkManager->sendCustomRequest(req, "DELETE", json.toJson()));
}

QString SpotifyRequests::errorMessage(QNetworkReply *reply)
{
    while (!reply->isFinished())
        QCoreApplication::processEvents();
    auto replyBody = reply->readAll();
    reply->deleteLater();
    if (replyBody.isEmpty())
        return QString();
    auto json = QJsonDocument::fromJson(replyBody).object();
    return json["error"].toObject()["message"].toString();
}

bool SpotifyRequests::refresh()
{
    // Make sure we have a refresh token

    auto refreshToken = credentials->refreshToken();
    if (refreshToken.isEmpty())
    {
        qDebug() << "Attempt to refresh without refresh token";
        return false;
    }

    // Create form
    auto postData = QString("grant_type=refresh_token&refresh_token=%1")
            .arg(refreshToken)
            .toUtf8();

    // Create request
    QNetworkRequest request(QUrl("https://accounts.spotify.com/api/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", "Basic " + QString("%1:%2")
                         .arg(credentials->clientId())
                         .arg(credentials->clientSecret()).toUtf8().toBase64());

    // Send request
    auto reply = networkManager->post(request, postData);
    while (!reply->isFinished())
        QCoreApplication::processEvents();

    // Parse json
    auto json = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();

    // Check if error
    if (json.contains("error_description") || !json.contains("access_token"))
    {
        QString error = json["error_description"].toString();
        QString msg;
        if(error.isEmpty())
            msg = QString("Failed to refresh token: {}").arg("no access token");
        else
            msg = QString("Failed to refresh token: {}").arg(error);
        qDebug() << msg;
        return false;
    }

    // Save as access token
    lastAuth = QDateTime::currentSecsSinceEpoch();
    auto accessToken = json["access_token"].toString();
    this->accessToken = accessToken;
    return true;
}

void SpotifyRequests::requestCurrentPlayback()
{
    auto context = new QObject();
    SpotifyRequests::connect(this, &SpotifyRequests::got, context, [this, context](const QJsonDocument &json)
    {
        delete context;
        emit gotPlayback(SpotifyTimelapse(json.object()));
    });
    getLater("me/player");
}

void SpotifyRequests::getLater(const QString &url)
{
    // Prepare fetch of request
    auto context = new QObject();
    QNetworkAccessManager::connect(networkManager, &QNetworkAccessManager::finished, context,
        [this, context, url](QNetworkReply *reply)
        {
            auto replyUrl = reply->url().toString();
            if (replyUrl.right(replyUrl.length() - 27) != url)
                return;
            delete context;
            // Parse reply as json
            auto json = QJsonDocument::fromJson(reply->readAll());
            reply->deleteLater();
            emit got(json);
        });

    networkManager->get(request(url));
}

bool SpotifyRequests::isValid() const
{
    return refreshValid;
}

QString SpotifyRequests::setVolume(int volume)
{
    return put(QString("me/player/volume?volume_percent=%1").arg(volume));
}

QString SpotifyRequests::seek(int position)
{
    return put(QString("me/player/seek?position_ms=%1").arg(position));
}

QJsonDocument SpotifyRequests::get(const QString &url)
{
    // Send request
    auto reply = networkManager->get(request(url));

    // Wait for request to finish
    while (!reply->isFinished())
        QCoreApplication::processEvents();

    // Parse reply as json
    auto json = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();

    // Return parsed json
    return json;
}

QVector<SpotifyDevice> SpotifyRequests::devices()
{
    auto json = getAsObject("me/player/devices");
    auto items = json["devices"].toArray();
    QVector<SpotifyDevice> devices(items.size());
    for (int i = 0; i < items.size(); i++)
        devices[i] = SpotifyDevice(items.at(i).toObject());
    return devices;
}

QString SpotifyRequests::setDevice(const SpotifyDevice &device)
{
    QVariantMap body;
    body["device_ids"] = QStringList({
        device.id
    });
    currentDevice = device.id;
    return put("me/player", &body);
}

QString SpotifyRequests::pause()
{
    return put("me/player/pause");
}

QString SpotifyRequests::playTracks(int trackIndex, const QString &context)
{
    QVariantMap body;
    body["context_uri"] = context;
    body["offset"] = QJsonObject({
        QPair<QString, int>("position", trackIndex)
    });

    return put(currentDevice == nullptr || currentDevice.isEmpty()
        ? QString("me/player/play")
        : QString("me/player/play?device_id=%1").arg(currentDevice), &body);
}


QString SpotifyRequests::playTracks(int trackIndex, const QStringList &all)
{
    QVariantMap body;
    body["uris"] = all;
    body["offset"] = QJsonObject({
        QPair<QString, int>("position", trackIndex)
    });

    return put(currentDevice == nullptr || currentDevice.isEmpty()
        ? QString("me/player/play")
        : QString("me/player/play?device_id=%1").arg(currentDevice), &body);
}

QString SpotifyRequests::playTracks(const QString &context)
{
    QVariantMap body;
    body["context_uri"] = context;

    return put(currentDevice == nullptr
        ? QString("me/player/play")
        : QString("me/player/play?device_id=%1").arg(currentDevice), &body);
}


QJsonObject SpotifyRequests::getAsObject(const QString &url)
{
    return get(url).object();
}

QJsonArray SpotifyRequests::getAsArray(const QString &url)
{
    return get(url).array();
}

QVector<Song> SpotifyRequests::search(const QString &query)
{
    QVector<Song> tracks;
    QJsonObject json = getAsObject(
                QString("search?q=%1&type=album,artist,playlist,track&limit=50&market=from_token")
                    .arg(query));

    tracks.reserve(json["tracks"].toObject()["total"].toInt());
    for (auto track : json["tracks"].toObject()["items"].toArray())
        tracks.append(Song(track.toObject()));

    return tracks;
}

QVector<Song> SpotifyRequests::tracks(const QString &query)
{
    QVector<Song> tracks;
    QJsonObject json = getAsObject(
                QString("tracks?ids=%1&limit=50&market=from_token")
                    .arg(query));
    tracks.reserve(50);
    for (auto track : json["tracks"].toArray()){
        tracks.append(Song(track.toObject()));
    }

    return tracks;
}

QByteArray SpotifyRequests::getImage(const QString &url)
{
    auto reply = networkManager->get(QNetworkRequest(QUrl(url)));
    while (!reply->isFinished())
        QCoreApplication::processEvents();
    reply->deleteLater();
    return reply->error() == QNetworkReply::NoError
        ? reply->readAll()
        : QByteArray();
}
