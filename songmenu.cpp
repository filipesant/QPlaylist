#include "songmenu.h"
#include <QDebug>

SongMenu::SongMenu(QString playlistId, QString songId, bool remove, bool add, QWidget *parent) :
    QMenu(parent), isRemove(remove), isAdd(add), songId(songId), playlistId(playlistId)
{
    if(isAdd){
        addPlaylist = addMenu(QPixmap("img/add_icon"), "Add to playlist");
        fillPlaylists(addPlaylist);
        QMenu::connect(addPlaylist, &QMenu::triggered, this, &SongMenu::addToPlaylist);
    }

    if(isRemove){
        removeFromPlaylist = addAction(QPixmap("img/remove_icon"), "Remove from playlist");
        QAction::connect(removeFromPlaylist, &QAction::triggered, this, &SongMenu::remFromPlaylist);
    }

}

void SongMenu::fillPlaylists(QMenu *addPlaylist)
{
    QString cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0];
    auto baseFile = QString("%1/playlist/").arg(cacheLocation);
    QDir playlistDir(baseFile);
    if(!playlistDir.isEmpty())
    {
        QStringList files = playlistDir.entryList(QStringList() << "*.json" ,QDir::Files);
        foreach(QString filename, files) {
            QFile file(QString("%1/playlist/%2").arg(cacheLocation).arg(filename));
            file.open(QIODevice::ReadOnly);
            auto json = QJsonDocument::fromJson(file.readAll());
            Playlist playlist = Playlist(json.object());
            file.close();

            // Create main action
            auto action = addPlaylist->addAction(playlist.name());
            action->setData(playlist.id());
        }
    }
}

void SongMenu::addToPlaylist(QAction *action)
{
    Playlist playList = Playlist(action->data().toString());
    playList.addSong(Song(songId));
    playList.save();
}

void SongMenu::remFromPlaylist(bool)
{    
    Playlist playList = Playlist(playlistId);
    Song song = Song(songId);
    playList.removeSong(song);
    playList.save();

}
