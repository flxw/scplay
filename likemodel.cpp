#include "likemodel.h"

LikeModel::LikeModel(const SoundStorage *storage, QObject *parent) :
    ListModelBase(storage,parent) {
    connect(soundStorage, SIGNAL(likesUpdated(QList<int>)), this, SLOT(updateLikes(QList<int>)));
}

// --- public methods
Sound LikeModel::getItem(const QModelIndex& index) const {
    if (index.row() < itemIds.size()) {
        return soundStorage->getSoundById(itemIds.at(index.row()));
    }
}

// --- public slots
void LikeModel::updateLikes(QList<int> newLikeIds) {
    itemIds = newLikeIds;

    emit dataChanged(QModelIndex(), QModelIndex());
}
