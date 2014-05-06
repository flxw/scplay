# include "soundstorage.h"
# include "soundcloudapi.h"

SoundStorage::SoundStorage(QObject *parent) : QObject(parent) {
    connect(&SoundCloudApi::getInstance(), SIGNAL(likesReceived(QList<Sound>)),
            this, SLOT(updateLikes(QList<Sound>)));

    connect(&SoundCloudApi::getInstance(), SIGNAL(playlistsReceived(QList<Sound>,QList<Playlist>)),
            this, SLOT(updatePlaylists(QList<Sound>,QList<Playlist>)));

    connect(&SoundCloudApi::getInstance(), SIGNAL(activitiesReceived(QList<QPair<int,QString> >,QList<Sound>,QList<Playlist>)),
            this, SLOT(updateActivities(QList<QPair<int,QString> >,QList<Sound>,QList<Playlist>)));
}

Sound SoundStorage::getSoundById(int id) const {
    return sounds.value(id);
}

Playlist SoundStorage::getPlaylistById(int id) const {
    return playlists.value(id);
}

// --- public slots
void SoundStorage::fill() {
    SoundCloudApi* api = &SoundCloudApi::getInstance();

    api->requestLikes();
    api->requestPlaylists();
    api->requestActivities();
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

void SoundStorage::updateActivities(QList<QPair<int, QString> > idsAndTypes, QList<Sound> sounds, QList<Playlist> playlists) {
    // integrate sounds into sound db
    for (QList<Sound>::const_iterator it = sounds.begin(); it != sounds.end(); ++it) {
        if (!this->sounds.contains(it->getId())) {
            this->sounds.insert(it->getId(), *it);
        }
    }

    // integrate playlists into playlist db
    for (QList<Playlist>::const_iterator it = playlists.begin(); it != playlists.end(); ++it) {
        this->playlists.insert(it->getId(), *it);
    }

    emit activitiesUpdated(idsAndTypes);
}
