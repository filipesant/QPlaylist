#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QAction>
#include "playercontroller.h"
#include "dialogcredentials.h"

PlayerController::PlayerController(QWidget *parent) : parent(parent)
{
    credentials = new SpotifyCredentials();
    spotify = new SpotifyRequests(credentials);

    m_layout = new Layout(parent);
    m_sideMenu = new SideMenu();
    m_playerControls = new PlayerControls();
    m_searchView = new SearchView();
    m_playListView = new PlaylistView();

    m_layout->AddSideMenu(m_sideMenu);
    m_layout->AddMediaPlayer(m_playerControls);

    m_stackedWidget = new QStackedWidget;
    m_stackedWidget->addWidget(m_searchView);
    m_stackedWidget->addWidget(m_playListView);
    m_layout->AddMainContent(m_stackedWidget);


    QAction *credentialsAction = parent->findChild<QAction *>("credentialsAction");
    createCacheLocation();

    QObject::connect(credentialsAction, &QAction::triggered, this, &PlayerController::credentialsDialog);
    QObject::connect(m_sideMenu, &SideMenu::SearchSong, this, &PlayerController::searchView);
    QObject::connect(m_sideMenu, &SideMenu::ShowPlaylists, this, &PlayerController::playlistView);

    QObject::connect(m_playerControls, &PlayerControls::pause, this, &PlayerController::pause);
    QObject::connect(m_playerControls, &PlayerControls::play, this, &PlayerController::play);
    QObject::connect(m_playerControls, &PlayerControls::next, this, &PlayerController::next);
    QObject::connect(m_playerControls, &PlayerControls::previous, this, &PlayerController::previous);
    QObject::connect(m_playerControls, &PlayerControls::changeVolume, this, &PlayerController::changeVolume);
    QObject::connect(m_playerControls, &PlayerControls::seek, this, &PlayerController::seek);


    auto timer = new QTimer(this);
    QTimer::connect(timer, &QTimer::timeout, this, &PlayerController::update);
    update();
    timer->start(1000);

    SpotifyRequests::connect(spotify, &SpotifyRequests::gotPlayback, this, &PlayerController::gotPlayback);

}

PlayerController::~PlayerController()
{
    delete m_layout;
    delete m_sideMenu;
    delete m_stackedWidget;
    delete m_searchView;
    delete m_playerControls;
    delete m_playListView;
}

QWidget* PlayerController::layout()
{
    return m_layout;
}

void PlayerController::searchView()
{
    m_stackedWidget->setCurrentWidget(m_searchView);
}

void PlayerController::playlistView(QJsonObject playlistJson)
{
    Playlist playlist = Playlist(playlistJson);
    m_playListView->setPlaylist(playlist);
    m_stackedWidget->setCurrentWidget(m_playListView);
}

void PlayerController::credentialsDialog()
{
    DialogCredentials *dlg = new DialogCredentials(parent);
    dlg->exec();
    delete dlg;
}

QString PlayerController::getCacheLocation()
{
    return m_cacheLocation;
}

void PlayerController::createCacheLocation()
{
    m_cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0];
    // Create main cache path and album subdir
    QDir cacheDir(m_cacheLocation);
    cacheDir.mkpath(".");
    cacheDir.mkdir("playlist");
}

void PlayerController::play()
{
    updateTracks();
    auto result = spotify->playTracks(m_playListView->getCurrentIndex(), m_tracks);
    if(result.isEmpty()){
        resetTimeLabesl();
        m_playerControls->setState(QMediaPlayer::PlayingState);
    }
}

void PlayerController::pause()
{
    auto result = spotify->pause();
    if(result.isEmpty())
        m_playerControls->setState(QMediaPlayer::PausedState);
}

void PlayerController::updateTracks()
{
    m_tracks.clear();
    m_tracks.reserve(m_playListView->getPlaylist().songs().size());
    foreach (Song song, m_playListView->getPlaylist().songs()) {
        m_tracks.append(QString("spotify:track:%1").arg(song.id));
    }
}

void PlayerController::next()
{
    updateTracks();
    auto result = spotify->playTracks(m_playListView->getCurrentIndex()+1, m_tracks);
    if(result.isEmpty()){
        resetTimeLabesl();
        m_playerControls->setState(QMediaPlayer::PlayingState);
        m_playListView->setCurrentIndex(m_playListView->getCurrentIndex()+1);
    }
}

void PlayerController::previous()
{
    updateTracks();
    auto result = spotify->playTracks(m_playListView->getCurrentIndex()-1, m_tracks);
    if(result.isEmpty()){
        resetTimeLabesl();
        m_playerControls->setState(QMediaPlayer::PlayingState);
        m_playListView->setCurrentIndex(m_playListView->getCurrentIndex()-1);
    }
}

void PlayerController::changeVolume(int level)
{
    auto result = spotify->setVolume(level);
    if(!result.isEmpty()){
        qDebug() << result << level;
    }
}


void PlayerController::seek(int pos)
{
    auto result = spotify->seek(pos);
    if(!result.isEmpty()){
        qDebug() << result << pos;
    }

}

void PlayerController::update()
{
    if (refreshCount < 0
            || ++refreshCount >= 3
            || m_timeLapse.progressMs + 1000 > m_timeLapse.item.duration)
    {
        spotify->requestCurrentPlayback();
        refreshCount = 0;
        return;
    }

    if (!m_timeLapse.isPlaying)
        return;
    m_timeLapse.progressMs += 1000;
    updated(m_timeLapse);
}

void PlayerController::updated(const SpotifyTimelapse &timeLapse)
{
    m_timeLapse = timeLapse;
    if (m_timeLapse.isPlaying){
        m_playerControls->setCurrentTime(formatTime(timeLapse.progressMs));
        m_playerControls->setFinalTime(formatTime(timeLapse.item.duration));
        m_playerControls->setValueSlider(timeLapse.progressMs);
        m_playerControls->setMaximumValueSlider(timeLapse.item.duration);

//        bool oldState = m_playerControls->setVolumeBlockSignals(true);
//        m_playerControls->setVolume(timeLapse.volume());
//        m_playerControls->setVolumeBlockSignals(oldState);

    }
}

void PlayerController::resetTimeLabesl()
{
    m_playerControls->setCurrentTime("00:00");
    m_playerControls->setFinalTime("00:00");
    m_playerControls->setValueSlider(0);
}

void PlayerController::gotPlayback(const SpotifyTimelapse &timeLapse)
{
    updated(timeLapse);
}

QString PlayerController::formatTime(int ms)
{
    auto duration = QTime(0, 0).addMSecs(ms);
    return QString("%1:%2")
            .arg(duration.minute())
            .arg(duration.second() % 60, 2, 10, QChar('0'));
}
