#ifndef MUSICMODEL_H
#define MUSICMODEL_H

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class Song
{
public:
    Song();
    explicit Song(const QJsonObject &item);
    explicit Song(const QString songId);
    QString id, album, albumId, artist, artistId, name, image, playlistId;
    int duration;
    bool isLocal, isPlayable;
    QDateTime addedAt;
    QJsonObject toJson() const;
private:
    QJsonValue getProperty(const QJsonObject &json, const QStringList &names);
};

#endif // MUSICMODEL_H
