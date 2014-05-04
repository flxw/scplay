#include "playlistmodel.h"

PlaylistModel::PlaylistModel(const SoundStorage *storage, QObject *parent) :
    ListModelBase(storage,parent) {
    connect(soundStorage, SIGNAL(playlistsUpdated(QList<int>)), this, SLOT(updatePlaylistIds(QList<int>)));
}

// --- public methods
Sound PlaylistModel::getItem(const QModelIndex& index) const {
    if (index.row() < itemIds.size()) {
        return soundStorage->getPlaylistById(itemIds.at(index.row()));
    }
}

// --- public slots
void PlaylistModel::updatePlaylistIds(QList<int> newPlaylistIds) {
    itemIds = newPlaylistIds;

    emit dataChanged(QModelIndex(), QModelIndex());
}
