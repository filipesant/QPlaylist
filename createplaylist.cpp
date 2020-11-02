#include "createplaylist.h"
#include "ui_dialogcreateplaylist.h"

DialogCreatePlayList::DialogCreatePlayList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCreatePlayList)
{
    ui->setupUi(this);
}

DialogCreatePlayList::~DialogCreatePlayList()
{
    delete ui;
}
