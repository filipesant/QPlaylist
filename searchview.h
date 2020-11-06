#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QWidget>
#include "spotifyrequests.h"
#include "songmenu.h"

namespace Ui {
class SearchView;
}

class SearchView : public QWidget
{
    Q_OBJECT

public:
    explicit SearchView(QWidget *parent = nullptr);
    ~SearchView();
private slots:
    void doSearch();
    void playTest();
    void popUpMenu(const QPoint &pos);

private:
    Ui::SearchView *ui;
    SpotifyRequests *spotify;
    SpotifyCredentials *credentials;
};

#endif // SEARCHVIEW_H
