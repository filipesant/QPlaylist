#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QJsonObject>

class SpotifyDevice
{
public:
    SpotifyDevice() = default;
    explicit SpotifyDevice(const QJsonObject &json);

    QString id;
    QString name;
    QString type;
    bool isActive = false;
    int volumePercent;
};
#endif // DEVICE_H
