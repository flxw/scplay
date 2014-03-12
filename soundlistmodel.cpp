# include "soundlistmodel.h"

SoundListModel::SoundListModel(QObject *parent) : QAbstractListModel(parent) { }

const SoundListItem &SoundListModel::getSongItem(QModelIndex &index) const {
    if (!index.isValid()) return SoundListItem();

    return soundItems.at(index.row());
}

QVariant SoundListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= soundItems.size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        SoundListItem sound = soundItems.at(index.row());
        return sound.getTitle();
    } else {
        return QVariant();
    }
}

QVariant SoundListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

int SoundListModel::rowCount(const QModelIndex &parent) const {
    return soundItems.count();
}
