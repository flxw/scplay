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
        case FEEDING_LIKES: return sounds[likeIds.at(index.row())]; break;
        case FEEDING_PLAYLISTS: return playlists[playlistIds.at(index.row())]; break;
        case FEEDING_PLAYLIST_SONGS: return sounds[playlists.value(currentPlaylistId).getSounds().at(index.row())];
    }
}

QVariant SoundModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

QVariant SoundModel::data(const QModelIndex &index, int role) const {
    return QVariant();
}

int SoundModel::rowCount(const QModelIndex&) const {
    switch (state) {
        case FEEDING_LIKES: return likeIds.count(); break;
        case FEEDING_PLAYLISTS: return playlistIds.count(); break;
        case FEEDING_PLAYLIST_SONGS: return playlists[currentPlaylistId].getSounds().count(); break;
    }
}

void SoundModel::switchToPlaylistSongFeed(const QModelIndex& index) {
    currentPlaylistId = playlistIds.at(index.row());
    state = FEEDING_PLAYLIST_SONGS;

    emit dataChanged(QModelIndex(), QModelIndex());
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

    // replace the hashed playlists with the new ones
    this->playlists.clear();
    this->playlistIds.clear();

    for (QList<Playlist>::const_iterator it = playlists.begin(); it != playlists.end(); ++it) {
        this->playlists.insert(it->getId(), *it);
        this->playlistIds.append(it->getId());
    }

    if ((state == FEEDING_PLAYLISTS) || (state == FEEDING_PLAYLIST_SONGS)) {
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void SoundModel::fill() {
    SoundCloudApi::getInstance().requestLikes();
    SoundCloudApi::getInstance().requestPlaylists();
}

void SoundModel::switchToPlaylistFeed() {
    if (state != FEEDING_PLAYLISTS) {
        state = FEEDING_PLAYLISTS;
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void SoundModel::switchToLikeFeed() {
    if (state != FEEDING_LIKES) {
        state = FEEDING_LIKES;
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}
