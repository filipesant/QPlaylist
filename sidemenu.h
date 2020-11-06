#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QWidget>
#include "spotifycredentials.h"

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
    void SearchSongClicked();

signals:
    void SearchSong();
    void ShowPlaylists();
};

#endif // SIDEMENU_H
