# include "soundmodel.h"

# include "soundcloudapi.h"

SoundModel::SoundModel(QObject *parent) : QAbstractListModel(parent) {
    currentPlaylistId = -1;
    state = FEEDING_LIKES;

    connect(&SoundCloudApi::getInstance(), SIGNAL(likesReceived(QList<Sound>)), this, SLOT(updateLikeData(QList<Sound>)));
    connect(&SoundCloudApi::getInstance(), SIGNAL(playlistsReceived(QList<Sound>,QList<Playlist>)),
            this, SLOT(updatePlaylists(QList<Sound>, QList<Playlist>)));
}

// --- public methods
Sound SoundModel::getItem(const QModelIndex& index) const {
    switch (state) {
        case FEEDING_LIKES: return sounds[likeIds.at(index.row())];
        default: Q_ASSERT("this state should not yet be used!!!");
    }
}

QVariant SoundModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

QVariant SoundModel::data(const QModelIndex &index, int role) const {
    return QVariant();
}

int SoundModel::rowCount(const QModelIndex &parent) const {
    switch (state) {
        case FEEDING_LIKES: return likeIds.count();
    }
}

// --- public slots
void SoundModel::updateLikeData(QList<Sound> newLikes) {
    likeIds.clear();

    for (QList<Sound>::const_iterator it = newLikes.begin(); it != newLikes.end(); ++it) {
        int id = it->getId();

        likeIds.append(it->getId());

        // save the sound if it is not yet present in our sound database
        if (!sounds.contains(id)) {
            sounds.insert(id, *it);
        }
    }

    if (state == FEEDING_LIKES) {
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void SoundModel::updateArtworkData(int id, QPixmap artwork) {
    sounds[id].setArtwork(artwork);
}

void SoundModel::updatePlaylists(QList<Sound> sounds, QList<Playlist> playlists) {
    // merge the sounds into the sound database
    for (QList<Sound>::const_iterator it = sounds.begin(); it != sounds.end(); ++it) {
        int id = it->getId();

        if (!this->sounds.contains(id)) {
            this->sounds.insert(id, *it);
        }
    }

    // simply update the playlist list
    this->playlists = playlists;

    if ((state == FEEDING_PLAYLISTS) || (state == FEEDING_PLAYLIST_SONGS)) {
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void SoundModel::fill() {
    SoundCloudApi::getInstance().getLikes();
    SoundCloudApi::getInstance().getPlaylists();
}
