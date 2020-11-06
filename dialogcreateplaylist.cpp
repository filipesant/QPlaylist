#include "dialogcreateplaylist.h"
#include "ui_dialogcreateplaylist.h"

DialogCreatePlayList::DialogCreatePlayList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCreatePlayList)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
}

DialogCreatePlayList::~DialogCreatePlayList()
{
    delete ui;
}
