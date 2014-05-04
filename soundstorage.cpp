# include "soundstorage.h"
# include "soundcloudapi.h"

SoundStorage::SoundStorage(QObject *parent) : QObject(parent) {
    connect(&SoundCloudApi::getInstance(), SIGNAL(likesReceived(QList<Sound>)),
            this, SLOT(updateLikes(QList<Sound>)));

    connect(&SoundCloudApi::getInstance(), SIGNAL(playlistsReceived(QList<Sound>,QList<Playlist>)),
            this, SLOT(updatePlaylists(QList<Sound>,QList<Playlist>)));
}

Sound SoundStorage::getSoundById(int id) const {
    return sounds.value(id);
}

Sound SoundStorage::getPlaylistById(int id) const {
    return playlists.value(id);
}

// --- public slots
void SoundStorage::fill() {
    SoundCloudApi* api = &SoundCloudApi::getInstance();

    api->requestLikes();
    api->requestPlaylists();
}

// --- private slots
void SoundStorage::updatePlaylists(QList<Sound> containedSongs, QList<Playlist> newPlaylists) {
    // merge the sounds into the sound database
    for (QList<Sound>::const_iterator it = containedSongs.begin(); it != containedSongs.end(); ++it) {
        int id = it->getId();

        if (!sounds.contains(id)) {
            sounds.insert(id, *it);
        }
    }

    // replace the hashed playlists with the new ones
    QList<int> playlistIds;
    playlists.clear();

    for (QList<Playlist>::const_iterator it = newPlaylists.begin(); it != newPlaylists.end(); ++it) {
        playlists.insert(it->getId(), *it);
        playlistIds.append(it->getId());
    }

    emit playlistsUpdated(playlistIds);
}

void SoundStorage::updateLikes(QList<Sound> newLikes) {
    QList<int> ids;

    for (QList<Sound>::const_iterator it = newLikes.begin(); it != newLikes.end(); ++it) {
        int id = it->getId();

        ids.append(it->getId());

        // save the sound if it is not yet present in our sound database
        if (!sounds.contains(id)) {
            sounds.insert(id, *it);
        }
    }

    emit likesUpdated(ids);
}
