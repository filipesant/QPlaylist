#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QWidget>
#include <QStackedWidget>
#include "layout.h"
#include "sidemenu.h"
#include "playercontrols.h"
#include "searchview.h"
#include "playlistview.h"

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
    ~PlayerController();

private slots:
    void play();

private:
    void createCacheLocation();
    QWidget *parent = nullptr;
    Layout *m_layout;
    QStackedWidget *m_stackedWidget;
    PlaylistView *m_playListView;
    SearchView *m_searchView;
    SideMenu *m_sideMenu;
    PlayerControls *m_playerControls;
    QString m_cacheLocation;

};

#endif // PLAYERCONTROLLER_H
