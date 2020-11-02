#include "playlistdelegate.h"
#include <QPainter>
#include <QFont>

PlaylistDelegate::PlaylistDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    m_iconSize = QSize(16,16);
    m_margins = QMargins(0, 0, 0, 0);
    m_spacingHorizontal = 0;
    m_spacingVertical = 0 ;

}

PlaylistDelegate::~PlaylistDelegate()
{
    //    delete m_ptr;
}

QSize PlaylistDelegate::iconSize() const
{
    return m_iconSize;
}

void PlaylistDelegate::setIconSize(int width, int height)
{
    m_iconSize = QSize(width, height);
}

QMargins PlaylistDelegate::contentsMargins() const
{
    return m_margins;
}

void PlaylistDelegate::setContentsMargins(int left, int top, int right, int bottom)
{
    m_margins = QMargins(left, top, right, bottom);
}

int PlaylistDelegate::horizontalSpacing() const
{
    return m_spacingHorizontal;
}

void PlaylistDelegate::setHorizontalSpacing(int spacing)
{
    m_spacingHorizontal = spacing;
}

int PlaylistDelegate::verticalSpacing() const
{
    return m_spacingVertical;
}

void PlaylistDelegate::setVerticalSpacing(int spacing)
{
    m_spacingVertical = spacing;
}


QRect PlaylistDelegate::songTitleBox(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QFont f(option.font);

    f.setPointSizeF(songTitleFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(index.data(Qt::UserRole).toString())
            .adjusted(0, 0, 1, 1);
}

qreal PlaylistDelegate::songTitleFontPointSize(const QFont &f) const
{
    return 0.85*f.pointSize();
}

QRect PlaylistDelegate::singerBox(const QStyleOptionViewItem &option) const
{
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);
    const QRect &contentRect(rect.adjusted(m_margins.left(),
                                           m_margins.top(),
                                           -m_margins.right(),
                                           -m_margins.bottom()));
    const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
    const bool hasIcon = !opt.icon.isNull();
    const int bottomEdge = rect.bottom();
    QFont f(opt.font);

    f.setPointSize(songTitleFontPointSize(opt.font));

    painter->save();
    painter->setClipping(true);
    painter->setClipRect(rect);
    painter->setFont(opt.font);

    // Draw background
    painter->fillRect(rect, opt.state & QStyle::State_Selected ?
                          palette.highlight().color() :
                          palette.light().color());

    // Draw bottom line
    painter->setPen(lastIndex ? palette.dark().color()
                              : palette.mid().color());
    painter->drawLine(lastIndex ? rect.left() : m_margins.left(),
                      bottomEdge, rect.right(), bottomEdge);

    // Draw message icon
    if (hasIcon)
        painter->drawPixmap(contentRect.left(), contentRect.top(),
                            opt.icon.pixmap(m_iconSize));

    // Draw timestamp
    QRect timeStampRect(songTitleBox(opt, index));

    timeStampRect.moveTo(m_margins.left() + m_iconSize.width()
                         + m_spacingHorizontal, contentRect.top());

    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(timeStampRect, Qt::TextSingleLine,
                      index.data(Qt::UserRole).toString());

    // Draw message text
    QRect messageRect(singerBox(opt));

    messageRect.moveTo(timeStampRect.left(), timeStampRect.bottom()
                       + m_spacingVertical);

    painter->setFont(opt.font);
    painter->setPen(palette.windowText().color());
    painter->drawText(messageRect, Qt::TextSingleLine, opt.text);

    painter->restore();
}

QSize PlaylistDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int textHeight = songTitleBox(opt, index).height()
            + m_spacingVertical + singerBox(opt).height();
    int iconHeight = m_iconSize.height();
    int h = textHeight > iconHeight ? textHeight : iconHeight;

    return QSize(opt.rect.width(), m_margins.top() + h
                 + m_margins.bottom());
}
