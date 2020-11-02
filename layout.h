#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QStackedWidget>

class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(QWidget *parent = nullptr);
    QWidget* getLayout();
    void AddSideMenu(QWidget *sideMenu = nullptr);
    void AddMediaPlayer(QWidget *mediaPlayer = nullptr);
    void AddMainContent(QWidget *content = nullptr);

private:
    QGridLayout *centralLayout;

signals:

};

#endif // LAYOUT_H
