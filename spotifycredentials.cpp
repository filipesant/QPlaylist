#include "spotifycredentials.h"

SpotifyCredentials::SpotifyCredentials()
{
    networkManager = new QNetworkAccessManager();
    load();
}

SpotifyCredentials::~SpotifyCredentials()
{
    delete networkManager;
}

QString SpotifyCredentials::authUrl(const QString &clientId, const QString &redirect)
{
    // Scopes for request, for clarity
    // For now, these are mostly identical to spotify-tui
    QStringList scopes = {
        "playlist-read-collaborative",
        "playlist-read-private",
        "playlist-modify-private",
        "playlist-modify-public",
        "user-follow-read",
        "user-follow-modify",
        "user-library-modify",
        "user-library-read",
        "user-modify-playback-state",
        "user-read-currently-playing",
        "user-read-playback-state",
        "user-read-private",
        "user-read-recently-played",
        "streaming",
        "user-read-email",
        "user-top-read"
    };

    // Prepare url and open browser
    QUrl redirectUrl(redirect);
    return QString("https://accounts.spotify.com/authorize?client_id=%1&response_type=code&redirect_uri=%2&scope=%3")
            .arg(clientId)
            .arg(QString(redirectUrl.toEncoded()))
            .arg(scopes.join("%20"));
}

QString SpotifyCredentials::auth(const QString &code, const QString &redirect, const QString &id, const QString &secret)
{
    if (code.isEmpty())
        return QString("no code specified");
    QUrl redirectUrl(redirect);

    // Prepare form to send
    auto postData = QString("grant_type=authorization_code&code=%1&redirect_uri=%2")
            .arg(code)
            .arg(QString(redirectUrl.toEncoded()))
            .toUtf8();

    // Prepare request
    QNetworkRequest request(QUrl("https://accounts.spotify.com/api/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization",
                         "Basic " + QString("%1:%2").arg(id).arg(secret).toUtf8().toBase64());

    // Send request
    auto reply = networkManager->post(request, postData);

    // Wait for response
    while (!reply->isFinished())
        QCoreApplication::processEvents();
    auto jsonData = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();
    if (jsonData.contains("error_description"))
        return jsonData["error_description"].toString();

    // Save access/refresh token to settings
    m_clientIdText = id;
    m_clientSecretText = secret;
    m_redirect = redirect;
    m_accessToken = jsonData["access_token"].toString();
    m_refreshToken = jsonData["refresh_token"].toString();
    save();

    // Everything hopefully went fine
    return QString();
}

QString SpotifyCredentials::clientId()
{
    return m_clientIdText;
}

QString SpotifyCredentials::clientSecret()
{
    return m_clientSecretText;
}

QString SpotifyCredentials::redirectUri()
{
    return m_redirect;
}

QString SpotifyCredentials::accessToken()
{
    return m_accessToken;
}

QString SpotifyCredentials::refreshToken()
{
    return m_refreshToken;
}

QString SpotifyCredentials::fileName()
{
    return QString("%1.json").arg(QStandardPaths::writableLocation(
                                      QStandardPaths::AppConfigLocation));
}

QString SpotifyCredentials::filePath()
{
    return QFileInfo(fileName()).absolutePath();
}

void SpotifyCredentials::fromJson(const QJsonObject &json)
{
    auto a = json["Credentials"].toObject();
    // Credentials
    setValue(a, "client_id", m_clientIdText);
    setValue(a, "client_secret", m_clientSecretText);
    setValue(a, "redirect_uri", m_redirect);
    setValue(a, "access_token", m_accessToken);
    setValue(a, "refresh_token", m_refreshToken);
}

void SpotifyCredentials::load()
{
    QFile file(fileName());
    file.open(QIODevice::ReadOnly);
    auto data = file.readAll();
    if (data.isEmpty())
    {
        file.close();
        return;
    }

    QJsonParseError error;
    auto json = QJsonDocument::fromJson(data, &error);
    file.close();
    if (error.error != QJsonParseError::NoError)
    {
        //Log::error("Unable to read JSON settings: {}", error.errorString());
        return;
    }

    fromJson(json.object());
}

QJsonObject SpotifyCredentials::toJson()
{
    return QJsonObject(
    {
                    QPair<QString, QJsonObject>("Credentials", {
                        {"client_id", m_clientIdText},
                        {"client_secret", m_clientSecretText},
                        {"redirect_uri", m_redirect},
                        {"access_token", m_accessToken},
                        {"refresh_token", m_refreshToken}
                    })
                });
}

void SpotifyCredentials::save()
{
    mutex.lock();
    QDir::root().mkpath(SpotifyCredentials::filePath());
    QFile file(fileName());
    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(toJson()).toJson(QJsonDocument::Indented));
    file.close();
    mutex.unlock();
}

template<typename T>
void SpotifyCredentials::setValue(const QJsonObject &json, const QString &key, T &value)
{
    if (!json.contains(key))
        return;

    if (!json[key].toVariant().canConvert<T>())
    {
        return;
    }

    value = json[key].toVariant().value<T>();
}
