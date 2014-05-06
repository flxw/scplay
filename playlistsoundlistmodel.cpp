# include "playlistsoundlistmodel.h"

PlaylistSoundListModel::PlaylistSoundListModel(const SoundStorage *storage, QObject *parent) :
    ListModelBase(storage,parent) {
}

// --- public methods
Sound PlaylistSoundListModel::getItem(const QModelIndex& index) const {
    if (index.row() < itemIds.size()) {
        return soundStorage->getSoundById(itemIds.at(index.row()));
    }
}

// --- public slots
void PlaylistSoundListModel::updateSoundIds(QList<int> newSoundIds) {
    itemIds = newSoundIds;

    emit dataChanged(QModelIndex(), QModelIndex());
}
