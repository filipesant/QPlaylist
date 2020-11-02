#include <QWidget>
#include <QDebug>
#include <QObject>
#include "playercontroller.h"

PlayerController::PlayerController(QWidget *parent)
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

    QObject::connect(m_sideMenu, &SideMenu::SearchSong, this, &PlayerController::searchView);
    QObject::connect(m_sideMenu, &SideMenu::ShowPlaylists, this, &PlayerController::playlistView);
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

void PlayerController::playlistView()
{
   m_stackedWidget->setCurrentWidget(m_playListView);
}
