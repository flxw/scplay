# include "sound.h"

Sound::Sound() {}

// --- public functions
const QPixmap &Sound::getArtwork() const {
    return artwork;
}

void Sound::setArtwork(QPixmap &p) {
    artwork = p;
}

bool Sound::hasArtwork() const {
    return !artwork.isNull();
}

const QUrl& Sound::getArtworkUrl() const {
    return artworkUrl;
}

void Sound::setArtworkUrl(QString url) {
    artworkUrl = url;
}

const QString& Sound::getTitle() const {
    return title;
}

void Sound::setTitle(QString t) {
    title = t;
}

const QString& Sound::getUser() const {
    return user;
}

void Sound::setUser(QString u) {
    user =u;
}

int Sound::getId() const {
    return id;
}

void Sound::setId(int i) {
    id = i;
}
