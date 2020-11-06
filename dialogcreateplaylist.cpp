#include "dialogcreateplaylist.h"
#include "ui_dialogcreateplaylist.h"

DialogCreatePlayList::DialogCreatePlayList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCreatePlayList)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
    QObject::connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(ui->pushButtonCreate, &QPushButton::clicked, this, &QDialog::accept);
}

DialogCreatePlayList::~DialogCreatePlayList()
{
    delete ui;
}

QString DialogCreatePlayList::plalistName()
{
    return ui->lineEdit->text();
}
