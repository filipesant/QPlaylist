#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QWidget>

namespace Ui {
class PlaylistView;
}

class PlaylistView : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistView(QWidget *parent = nullptr);
    ~PlaylistView();
    void addMessage(const QString &text, const QPixmap &pixmap,
                             const QDateTime &dateTime);
    void clearAll();

private:
    Ui::PlaylistView *ui;
};

#endif // PLAYLISTVIEW_H
