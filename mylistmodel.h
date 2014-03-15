#ifndef SOUNDLISTMODEL_H
#define SOUNDLISTMODEL_H

# include <QAbstractListModel>
# include <QList>

# include "listitem.h"

class MyListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MyListModel(QObject *parent = 0);

    virtual ListItem getItem(const QModelIndex& index) const = 0;
    virtual QVariant data(const QModelIndex &index, int role) const = 0;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

protected slots:
    void updateModel(QList<int> newItems);

    // --- attributes
protected:
    QList<int> dataIds;
};

#endif // SOUNDLISTMODEL_H
