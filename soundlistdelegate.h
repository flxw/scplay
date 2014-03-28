#ifndef SOUNDLISTDELEGATE_H
#define SOUNDLISTDELEGATE_H

# include <QStyledItemDelegate>

class SoundListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SoundListDelegate(QObject *parent = 0);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // SOUNDLISTDELEGATE_H
