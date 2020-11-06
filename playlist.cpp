#include "playlist.h"
#include <QDebug>

Playlist::Playlist()
{
    m_name = QString();
}

Playlist::Playlist(const QJsonObject &json)
{
    m_id = json.contains("id") ? json["id"].toString() : QString();
    m_name = json.contains("name") ? json["name"].toString() : QString();

    if(json.contains("songs")){
        foreach (auto song, json["songs"].toArray()) {
            m_songs.append(Song(song["song"].toObject()));
        }
    }
}

Playlist::Playlist(const QString playlistId)
{
    auto json = findById(playlistId);
    m_id = json.contains("id") ? json["id"].toString() : QString();
    m_name = json.contains("name") ? json["name"].toString() : QString();

    if(json.contains("songs")){
        foreach (auto song, json["songs"].toArray()) {
            m_songs.append(Song(song["song"].toObject()));
        }
    }
}


QVector<Song> Playlist::songs()
{
    return m_songs;
}

QString Playlist::name()
{
    return m_name;
}

QString Playlist::id()
{
    return m_id;
}

void Playlist::setName(QString name)
{
    m_name = name;
    m_id = name.replace(" ","");
}

void Playlist::addSong(Song song)
{
    song.playlistId = m_id;
    m_songs.append(song);
}

void Playlist::removeSong(Song song)
{
    for (int i = 0; i < m_songs.size() ; i++) {
        if(m_songs[i].id.contains(song.id)){
            m_songs.remove(i);
            i = 0;
        }
    }
}

QJsonObject Playlist::toJson()
{
    return QJsonObject({QPair<QString, QString>("name", m_name),
                        QPair<QString, QString>("id", m_id),
                        QPair<QString, int>("total", 0),
                        QPair<QString, QJsonArray>("songs", songsToJson())
                       });
}

QJsonArray Playlist::songsToJson()
{
    for (auto &song : m_songs)
        m_songsArray.append(song.toJson());

    return m_songsArray;
}

void Playlist::save()
{
    QString cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0];
    auto baseFile = QString("%1/playlist/%2").arg(cacheLocation).arg(m_id);
    if (QFileInfo::exists(baseFile))
        QFile(baseFile).remove();
    // Save new
    QJsonDocument json(toJson());
    QFile file(QString("%1.json").arg(baseFile));
    file.open(QIODevice::WriteOnly);
    file.write(json.toJson());
}

QJsonObject Playlist::findById(QString playlistId)
{
    cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0];
    QFile file(QString("%1/playlist/%2.json").arg(cacheLocation).arg(playlistId));
    file.open(QIODevice::ReadOnly);
    auto json = QJsonDocument::fromJson(file.readAll());
    file.close();

    return json.object();
}



