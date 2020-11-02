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

    PlaylistDelegate *delegate = new PlaylistDelegate(ui->listViewPlaylist);
    delegate->setContentsMargins(8, 8, 8, 8);
    delegate->setIconSize(32, 32);
    delegate->setHorizontalSpacing(8);
    delegate->setVerticalSpacing(4);

    ui->listViewPlaylist->setModel(new QStandardItemModel(this));
    ui->listViewPlaylist->setItemDelegate(delegate);

    addMessage(tr("This is some text of an info message"),
               QPixmap(":img/not-found_icon.png"),
               QDateTime::currentDateTime());
}

PlaylistView::~PlaylistView()
{
    delete ui;
}

void PlaylistView::addMessage(const QString &text, const QPixmap &pixmap,
                              const QDateTime &dateTime)
{
    auto *item = new QStandardItem(QIcon(pixmap), text);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(dateTime.toString("yyyy-MM-dd"), Qt::UserRole);

    static_cast<QStandardItemModel *>(ui->listViewPlaylist->model())->appendRow(item);
    ui->listViewPlaylist->scrollToBottom();
}

void PlaylistView::clearAll()
{
    static_cast<QStandardItemModel *>(ui->listViewPlaylist->model())->clear();
}


