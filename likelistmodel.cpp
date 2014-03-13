# include "likelistmodel.h"
# include "soundcloudapi.h"

LikeListModel::LikeListModel(QObject *parent) : SoundListModel(parent)
{
}

// --- protected functions
void LikeListModel::fillModel() {
    SoundCloudApi& scApi = SoundCloudApi::getInstance();

    connect(&scApi, SIGNAL(likesReceived(QList<SoundListItem>)), this, SLOT(updateModel(QList<SoundListItem>)));

    scApi.getLikes();
}

void LikeListModel::updateModel(QList<SoundListItem> newItems) {
    this->soundItems = newItems;

    emit dataChanged(QModelIndex(), QModelIndex());
}
