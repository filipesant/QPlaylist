#include "layout.h"
#include <QBoxLayout>
#include <QLabel>
#include <QStackedWidget>

Layout::Layout(QWidget *parent) : QWidget(parent)
{    
    centralLayout = new QGridLayout;
    centralLayout->setHorizontalSpacing(0);
    centralLayout->setVerticalSpacing(0);
    this->setLayout(centralLayout);
}

void Layout::AddSideMenu(QWidget *sideMenu)
{
    centralLayout->addWidget(sideMenu, 0, 0, 2, 1);
}

void Layout::AddMainContent(QWidget *content)
{
    centralLayout->addWidget(content, 0, 1);

}

void Layout::AddMediaPlayer(QWidget *mediaPlayer)
{
    centralLayout->addWidget(mediaPlayer, 1, 1);
}


