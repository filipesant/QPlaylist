#include "searchview.h"
#include "ui_searchview.h"
#include "playlistdelegate.h"
#include <QDebug>
#include <QStandardItemModel>

SearchView::SearchView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchView)
{
    ui->setupUi(this);credentials = new SpotifyCredentials();
    spotify = new SpotifyRequests(credentials);

    PlaylistDelegate *delegate = new PlaylistDelegate(ui->listViewSongs);
    delegate->setContentsMargins(8, 8, 8, 8);
    delegate->setIconSize(32, 32);
    delegate->setHorizontalSpacing(8);
    delegate->setVerticalSpacing(4);

    ui->listViewSongs->setModel(new QStandardItemModel(this));
    ui->listViewSongs->setItemDelegate(delegate);

    ui->listViewSongs->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    QLabel::connect(ui->listViewSongs, &QWidget::customContextMenuRequested, this, &SearchView::popUpMenu);
    QObject::connect(ui->lineEditSearch, &QLineEdit::returnPressed, this, &SearchView::doSearch);
//    QObject::connect(ui->listViewSongs, &QListView::clicked, this, &SearchView::playTest);
}

SearchView::~SearchView()
{
    delete ui;
}

void SearchView::doSearch()
{
    QVector<Song> result =  spotify->search(ui->lineEditSearch->text());
    ui->listViewSongs->model()->removeRows(0,ui->listViewSongs->model()->rowCount());
    for (Song &track : result)
    {
        QPixmap img;
        img.loadFromData(spotify->getImage(track.image), "jpeg");
        auto *item = new QStandardItem(img, track.name);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setData(track.artist, Qt::UserRole);
        item->setData(track.id, Qt::AccessibleDescriptionRole);
        static_cast<QStandardItemModel *>(ui->listViewSongs->model())->appendRow(item);
    }

}

void SearchView::playTest()
{
    QModelIndex index = ui->listViewSongs->currentIndex();
    //    static_cast<QStandardItemModel *>(index)->data();
    qDebug() << index.data().toString();
    spotify->playTracks(index.data().toString());
}

void SearchView::popUpMenu(const QPoint &pos)
{
    QString songId = QString();
    if(ui->listViewSongs->currentIndex().isValid())
        songId = ui->listViewSongs->currentIndex().data(Qt::AccessibleDescriptionRole).toString();
    (new SongMenu("",songId, false,true,this))->popup(ui->listViewSongs->mapToGlobal(pos));
    qDebug() << "AQUI";
}


