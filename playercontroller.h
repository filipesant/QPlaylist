#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QWidget>
#include <QStackedWidget>
#include "layout.h"
#include "sidemenu.h"
#include "playercontrols.h"
#include "searchview.h"
#include "playlistview.h"
#include "spotifycredentials.h"
#include "spotifyrequests.h"
#include "spotifytimelapse.h"

class PlayerController : public QObject
{
    Q_OBJECT

public:
    PlayerController(QWidget *parent = nullptr);
    QWidget* layout();
    void searchView();
    void playlistView(QJsonObject playlistJson);
    void credentialsDialog();
    QString getCacheLocation();
    void update();
    void updated(const SpotifyTimelapse &timeLapse);
    ~PlayerController();

private slots:
    void pause();
    void play();
    void next();
    void previous();
    void changeVolume(int level);
    void seek(int pos);
    void gotPlayback(const SpotifyTimelapse &timeLapse);
    void resetTimeLabesl();

private:
    void createCacheLocation();
    QString formatTime(int ms);
    QWidget *parent = nullptr;
    Layout *m_layout;
    QStackedWidget *m_stackedWidget;
    PlaylistView *m_playListView;
    SearchView *m_searchView;
    SideMenu *m_sideMenu;
    PlayerControls *m_playerControls;
    QString m_cacheLocation;
    SpotifyRequests *spotify;
    SpotifyCredentials *credentials;
    QStringList m_tracks;
    SpotifyTimelapse m_timeLapse;
    int refreshCount = -1;

    void updateTracks();
};

#endif // PLAYERCONTROLLER_H
