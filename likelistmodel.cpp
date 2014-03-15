# include "likelistmodel.h"
# include "soundcloudapi.h"
# include "datastore.h"

LikeListModel::LikeListModel(QObject *parent) : MyListModel(parent) {
    connect(&DataStore::getInstance(), SIGNAL(likesUpdated(QList<int>)), this, SLOT(updateModel(QList<int>)));
}

ListItem LikeListModel::getItem(const QModelIndex &index) const {
    if (!index.isValid()) return ListItem();

    const ListItem& ret = DataStore::getInstance().getSound(dataIds.at(index.row()));

    return ret;
}

QVariant LikeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= dataIds.size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const ListItem& sound = getItem(index);
        QString t = sound.getTitle();
        return t;
    } else {
        return QVariant();
    }
}
