#include "spotifyrequests.h"

SpotifyRequests::SpotifyRequests(QObject *parent) : QObject(parent)
{

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
    //    if (reply.contains("No active device found"))
    //    {
    //        auto d = devices();
    //        if (d.length() == 1)
    //        {
    //            setDevice(devices().at(0));
    //            return put(url, body);
    //        }
    //        else if (d.length() > 1)
    //        {
    //            DeviceSelectDialog dialog(d);
    //            if (dialog.exec() == QDialog::Accepted)
    //            {
    //                auto selected = dialog.selectedDevice();
    //                if (!selected.id.isEmpty())
    //                {
    //                    setDevice(selected);
    //                    return put(url, body);
    //                }
    //            }
    //        }
    //    }
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

    //    auto refreshToken = QString("settings.account.refreshToken");
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
                         .arg(clientId)
                         .arg(clientSecret).toUtf8().toBase64());

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
        auto error = json["error_description"].toString();
        //        QString msg = QString("Failed to refresh token: {}", error.isEmpty()
        //            ? "no access token"
        //            : error);
        return false;
    }

    // Save as access token
    lastAuth = QDateTime::currentSecsSinceEpoch();
    auto accessToken = json["access_token"].toString();
    this->accessToken = accessToken;
    //    settings.account.accessToken = accessToken;
    //    settings.save();
    return true;
}

bool SpotifyRequests::isValid() const
{
    return refreshValid;
}
