#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QWidget>
#include <QStandardItemModel>
#include "spotifycredentials.h"
#include "playlist.h"
#include "songmenu.h"

namespace Ui {
class SideMenu;
}

class SideMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SideMenu(QWidget *parent = nullptr);
    ~SideMenu();

private:
    Ui::SideMenu *ui;
    SpotifyCredentials *credentials;

private slots:
    void CreatePlayListClicked();
    void PlaylistClicked();
    void SearchSongClicked();
    void LoadPlaylists();
    void insertPlaylist(Playlist playlist);

signals:
    void SearchSong();
    void ShowPlaylists(QJsonObject playlistJson);
};

#endif // SIDEMENU_H
