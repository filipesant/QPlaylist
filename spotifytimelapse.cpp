#include "spotifytimelapse.h"

SpotifyTimelapse::SpotifyTimelapse(const QJsonObject &json)
{
    if (json["item"].isNull())
        return;
    progressMs = json["progress_ms"].toInt();
    item = Song(json["item"].toObject());
    isPlaying = json["is_playing"].toBool();
    device = SpotifyDevice(json["device"].toObject());
}

QVariantMap SpotifyTimelapse::metadata() const
{
    QString itemName(isPlaying ? item.name : "");
    QStringList itemArtist(isPlaying ? item.artist : "");
    QString itemAlbum(isPlaying ? item.album : "");
    QString itemId(isPlaying ? item.id : "");
    auto itemDuration = isPlaying ? item.duration : 0;
    QString itemImage(isPlaying ? item.image : "");

    auto metadata = QVariantMap({
        {"xesam:title", itemName},
        {"xesam:artist", itemArtist},
        {"xesam:album", itemAlbum},
        {"xesam:albumArtist", itemArtist},
        {"xesam:url", QString("https://open.spotify.com/track/%1").arg(itemId)},
        {"mpris:length", (qulonglong) itemDuration * 1000},
        {"mpris:artUrl", itemImage},
        {"mpris:trackid", QString("spotify:track:%1").arg(itemId)},
    });
    return metadata;
}

QJsonObject SpotifyTimelapse::toJson() const
{
    return QJsonObject(
        {
            QPair<QString, int>("progress_ms", progressMs),
            QPair<QString, QJsonObject>("item", item.toJson()),
            QPair<QString, bool>("is_playing", isPlaying)
        }
    );
}

int SpotifyTimelapse::volume() const
{
    return device.volumePercent;
}
