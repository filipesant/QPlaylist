#include "searchview.h"
#include "ui_searchview.h"

SearchView::SearchView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchView)
{
    ui->setupUi(this);
}

SearchView::~SearchView()
{
    delete ui;
}
