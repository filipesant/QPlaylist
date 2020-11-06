#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"
#include <QVector>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include <QStandardPaths>

class Playlist
{
public:
    Playlist();
    explicit Playlist(const QJsonObject &item);
    explicit Playlist(const QString plalistId);
    QVector<Song> songs();
    QString name();
    QString id();
    void setName(QString name);
    void addSong(Song song);
    void removeSong(Song song);
    QJsonObject toJson();
    QJsonObject findById(QString playlistId);
    void save();
private:
    QVector<Song> m_songs;
    QString m_name;
    QString m_id;
    QJsonArray songsToJson();
    QString cacheLocation;
    QJsonArray m_songsArray;
};

#endif // PLAYLIST_H
