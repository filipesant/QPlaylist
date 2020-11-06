#ifndef DIALOGCREATEPLAYLIST_H
#define DIALOGCREATEPLAYLIST_H

#include <QDialog>

namespace Ui {
class DialogCreatePlayList;
}

class DialogCreatePlayList : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreatePlayList(QWidget *parent = nullptr);
    ~DialogCreatePlayList();
    QString plalistName();

private:
    Ui::DialogCreatePlayList *ui;
};

#endif // DIALOGCREATEPLAYLIST_H
