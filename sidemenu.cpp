#include "sidemenu.h"
#include "ui_sidemenu.h"
#include <QDialog>
#include <QMessageBox>
#include <QPropertyAnimation>
#include "createplaylist.h"

SideMenu::SideMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideMenu)
{
    ui->setupUi(this);
    connect(ui->pushButtonCreatePlaylist, &QAbstractButton::clicked, this, &SideMenu::CreatePlayListClicked);
    connect(ui->pushButtonSearch, &QAbstractButton::clicked, this, &SideMenu::SearchSongClicked);
}

SideMenu::~SideMenu()
{
    delete ui;
}

void SideMenu::CreatePlayListClicked()
{
    DialogCreatePlayList *dlg = new DialogCreatePlayList(this);
    dlg->setModal(true);
    dlg->exec();

    emit ShowPlaylists();
}

void SideMenu::SearchSongClicked()
{
    emit SearchSong();
}
