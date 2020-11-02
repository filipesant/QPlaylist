#include "playlistview.h"
#include "ui_playlistview.h"

PlaylistView::PlaylistView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistView)
{
    ui->setupUi(this);
}

PlaylistView::~PlaylistView()
{
    delete ui;
}
