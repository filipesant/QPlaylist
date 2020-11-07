#include "playlistview.h"
#include "ui_playlistview.h"
#include "playlistdelegate.h"
#include <QStandardItemModel>
#include <QDateTime>

PlaylistView::PlaylistView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistView)
{
    ui->setupUi(this);

    credentials = new SpotifyCredentials();
    spotify = new SpotifyRequests(credentials);

    PlaylistDelegate *delegate = new PlaylistDelegate(ui->listViewPlaylist);
    delegate->setContentsMargins(8, 8, 8, 8);
    delegate->setIconSize(32, 32);
    delegate->setHorizontalSpacing(8);
    delegate->setVerticalSpacing(4);

    ui->listViewPlaylist->setModel(new QStandardItemModel(this));
    ui->listViewPlaylist->setItemDelegate(delegate);

    ui->listViewPlaylist->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    QLabel::connect(ui->listViewPlaylist, &QWidget::customContextMenuRequested, this, &PlaylistView::popUpMenu);
    QObject::connect(ui->listViewPlaylist, &QListView::clicked, this, &PlaylistView::PlaylistClicked);

}

PlaylistView::~PlaylistView()
{
    delete ui;
}

void PlaylistView::addSongs(const QString &text, const QPixmap &pixmap,
                              const QDateTime &dateTime)
{
    auto *item = new QStandardItem(QIcon(pixmap), text);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(dateTime.toString("yyyy-MM-dd"), Qt::UserRole);

    static_cast<QStandardItemModel *>(ui->listViewPlaylist->model())->appendRow(item);
    ui->listViewPlaylist->scrollToBottom();
}

Playlist PlaylistView::getPlaylist()
{
    return Playlist(m_playlistId);
}

Song PlaylistView::getCurrentSong()
{
    if(ui->listViewPlaylist->currentIndex().isValid())
    {
        QString songId = ui->listViewPlaylist->currentIndex().model()->data(ui->listViewPlaylist->currentIndex(),Qt::AccessibleDescriptionRole).toString();
        return Song(songId);
    }

    return Song();
}

int PlaylistView::getCurrentIndex()
{
    if(ui->listViewPlaylist->currentIndex().isValid())
    {
        return ui->listViewPlaylist->currentIndex().row();
    }

    return -1;
}

void PlaylistView::setCurrentIndex(int index)
{
    QModelIndex modelIndex = ui->listViewPlaylist->model()->index(index,0);
    ui->listViewPlaylist->setCurrentIndex(modelIndex);
}

void PlaylistView::clearAll()
{
    static_cast<QStandardItemModel *>(ui->listViewPlaylist->model())->clear();
}

void PlaylistView::setPlaylist(Playlist playlist)
{
    QString query;
    m_playlistId = playlist.id();
    foreach (Song song, playlist.songs())
    {
       query.append(song.id).append(",");
    };

    int index = query.lastIndexOf(QChar(','));
    QVector<Song> result = spotify->tracks(query.left(index));

    clearAll();

    for (Song &track : result)
    {
        QPixmap img;
        img.loadFromData(spotify->getImage(track.image), "jpeg");
        auto *item = new QStandardItem(img, track.name);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setData(track.artist, Qt::UserRole);
        item->setData(track.id, Qt::AccessibleDescriptionRole);
        static_cast<QStandardItemModel *>(ui->listViewPlaylist->model())->appendRow(item);
    }

    ui->labelPlaylistName->setText(playlist.name());
}

void PlaylistView::popUpMenu(const QPoint &pos)
{
    QString songId = QString();
    if(ui->listViewPlaylist->currentIndex().isValid())
        songId = ui->listViewPlaylist->currentIndex().model()->data(ui->listViewPlaylist->currentIndex(),Qt::AccessibleDescriptionRole).toString();
    if(ui->listViewPlaylist->model()->rowCount() > 0)
        (new SongMenu(m_playlistId,songId,true,false,this))->popup(ui->listViewPlaylist->mapToGlobal(pos));
}

void PlaylistView::PlaylistClicked()
{
    emit PlaylistSelected(m_playlistId);
}


