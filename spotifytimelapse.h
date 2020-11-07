#ifndef SPOTIFYTIMELAPSE_H
#define SPOTIFYTIMELAPSE_H

#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include "spotifydevice.h"
#include "song.h"

class SpotifyTimelapse
{
public:
public:
    SpotifyTimelapse() = default;
    explicit SpotifyTimelapse(const QJsonObject &json);

    QVariantMap metadata() const;
    QJsonObject toJson() const;
    int volume() const;
    Song item = Song();
    bool isPlaying = false;
    int progressMs = 0;
    SpotifyDevice device;
};

#endif // SPOTIFYTIMELAPSE_H
