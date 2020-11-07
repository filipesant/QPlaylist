#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QWidget>
#include "playlist.h"
#include "songmenu.h"
#include "spotifyrequests.h"

namespace Ui {
class PlaylistView;
}

class PlaylistView : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistView(QWidget *parent = nullptr);
    ~PlaylistView();
    void setPlaylist(Playlist playlist);
    void addSongs(const QString &text, const QPixmap &pixmap,
                             const QDateTime &dateTime);
    void clearAll();
    Playlist getPlaylist();
    Song getCurrentSong();
    int getCurrentIndex();
    void setCurrentIndex(int index);

private slots:
       void popUpMenu(const QPoint &pos);
       void PlaylistClicked();

signals:
       void PlaylistSelected(QString playlistId);

private:
    Ui::PlaylistView *ui;
    SpotifyRequests *spotify;
    SpotifyCredentials *credentials;
    QString m_playlistId;
};

#endif // PLAYLISTVIEW_H
