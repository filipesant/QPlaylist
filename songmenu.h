#ifndef SONGMENU_H
#define SONGMENU_H

#include <QWidget>
#include <QDesktopServices>
#include <QMenu>
#include <QDir>
#include <QJsonDocument>
#include "playlist.h"

class SongMenu : public QMenu
{
    Q_OBJECT

public:
    explicit SongMenu(QString playlistId = QString(),QString songId = QString(), bool remove = false, bool Add =false,QWidget *parent = nullptr);
    
private:
    void fillPlaylists(QMenu *addPlaylist);
    void addToPlaylist(QAction *action);
    void remFromPlaylist(bool checked);
    QMenu *addPlaylist;
    QAction *removeFromPlaylist;
    bool isAdd, isRemove;
    QString songId;
    QString playlistId;

signals:

};

#endif // SONGMENU_H
