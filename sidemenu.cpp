#include "sidemenu.h"
#include "ui_sidemenu.h"
#include <QDialog>
#include <QMessageBox>
#include <QPropertyAnimation>
#include "dialogcreateplaylist.h"
#include "playlist.h"

SideMenu::SideMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideMenu)
{
    ui->setupUi(this);
    credentials = new SpotifyCredentials();

    ui->listViewPlaylist->setModel(new QStandardItemModel(this));
    LoadPlaylists();

    connect(ui->pushButtonCreatePlaylist, &QAbstractButton::clicked, this, &SideMenu::CreatePlayListClicked);
    connect(ui->pushButtonSearch, &QAbstractButton::clicked, this, &SideMenu::SearchSongClicked);
    connect(ui->listViewPlaylist, &QListView::clicked, this, &SideMenu::PlaylistClicked);
}

SideMenu::~SideMenu()
{
    delete ui;
}

void SideMenu::CreatePlayListClicked()
{
    DialogCreatePlayList *dlg = new DialogCreatePlayList(this);
    int ret = dlg->exec();
    if(ret == QDialog::Accepted){
        Playlist *list = new Playlist();
        list->setName(dlg->plalistName());
        list->save();
        insertPlaylist(*list);
        emit ShowPlaylists(list->toJson());
        delete list;
    }
    delete dlg;
}

void SideMenu::LoadPlaylists()
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
            file.close();
            insertPlaylist(Playlist(json.object()));
        }
    }
}

void SideMenu::insertPlaylist(Playlist playlist)
{
    auto *item = new QStandardItem(playlist.name());
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(playlist.id(), Qt::UserRole);

    static_cast<QStandardItemModel *>(ui->listViewPlaylist->model())->appendRow(item);
    ui->listViewPlaylist->scrollToBottom();

}

void SideMenu::SearchSongClicked()
{
    emit SearchSong();
}

void SideMenu::PlaylistClicked()
{
    QString cacheLocation = QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0];
    auto playlistId = ui->listViewPlaylist->currentIndex().data(Qt::UserRole).toString();
    QFile file(QString("%1/playlist/%2.json").arg(cacheLocation).arg(playlistId));
    file.open(QIODevice::ReadOnly);
    auto json = QJsonDocument::fromJson(file.readAll());
    file.close();
    emit ShowPlaylists(json.object());
}
