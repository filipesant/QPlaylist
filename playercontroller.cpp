#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QAction>
#include "playercontroller.h"
#include "dialogcredentials.h"

PlayerController::PlayerController(QWidget *parent) : parent(parent)
{
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

    QObject::connect(m_playerControls, &PlayerControls::play, this, &PlayerController::play);
    QObject::connect(m_playerControls, &PlayerControls::next, this, &PlayerController::play);
    QObject::connect(m_playerControls, &PlayerControls::previous, this, &PlayerController::play);
    QObject::connect(m_playerControls, &PlayerControls::changeVolume, this, &PlayerController::play);

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
    qDebug() << "play";
}
