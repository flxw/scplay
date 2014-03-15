# include "datastore.h"
# include "soundcloudapi.h"

// --- public methods
DataStore& DataStore::getInstance() {
    static DataStore instance; // Guaranteed to be destroyed.
                           // Instantiated on first use.
    return instance;
}

SoundItem DataStore::getSound(int id) const {
    return songs.value(id);
}

// --- public slots
void DataStore::initiallyFillStore() {
    SoundCloudApi::getInstance().getLikes();
}

void DataStore::updateLikes(QList<SoundItem> newLikes) {
    QList<int> likeIds;
    int id;

    for (QList<SoundItem>::const_iterator it = newLikes.begin(); it != newLikes.end(); ++it) {
        id = it->getId();

        likeIds.append(it->getId());

        if (!songs.contains(id)) songs.insert(id, *it);
    }

    emit likesUpdated(likeIds);
}

void DataStore::updateArtwork(int id, QPixmap artwork) {
    songs[id].setArtwork(artwork);
}

// --- private methods
DataStore::DataStore() {
}
