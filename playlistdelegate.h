#ifndef PLAYLISTDELEGATE_H
#define PLAYLISTDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <qglobal.h>

class PlaylistDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PlaylistDelegate(QObject *parent = nullptr);
    ~PlaylistDelegate();

    QSize iconSize() const;
    void setIconSize(int width, int height);

    QMargins contentsMargins() const;
    void setContentsMargins(int left, int top, int right, int bottom);

    int horizontalSpacing() const;
    void setHorizontalSpacing(int spacing);

    int verticalSpacing() const;
    void setVerticalSpacing(int spacing);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    QSize m_iconSize;
    QMargins m_margins;
    int m_spacingHorizontal;
    int m_spacingVertical;

    inline QRect songTitleBox(const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    inline qreal songTitleFontPointSize(const QFont &f) const;
    inline QRect singerBox(const QStyleOptionViewItem &option) const;

signals:

};

#endif // PLAYLISTDELEGATE_H
