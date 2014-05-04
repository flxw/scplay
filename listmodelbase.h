#ifndef LISTMODELBASE_H
#define LISTMODELBASE_H

#include <QAbstractListModel>

# include "sound.h"
# include "soundstorage.h"

class ListModelBase : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ListModelBase(const SoundStorage *storage, QObject *parent = 0);

    virtual Sound getItem(const QModelIndex& index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role) const;

    int rowCount(const QModelIndex& = QModelIndex()) const;


    // --- attributes
protected:
    QList<int> itemIds;

    const SoundStorage* soundStorage;
};

#endif // LISTMODELBASE_H
