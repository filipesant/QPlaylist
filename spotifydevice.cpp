#include "spotifydevice.h"

SpotifyDevice::SpotifyDevice(const QJsonObject &json)
{
    id = json["id"].toString();
    name = json["name"].toString();
    type = json["type"].toString();
    isActive = json["is_active"].toBool();
    volumePercent = json["volume_percent"].toInt();
}
