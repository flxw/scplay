# include "playlist.h"

Playlist::Playlist(QList<int> &sounds) {
    soundIds = sounds;
}

const QList<int>& Playlist::getSounds() const {
    return soundIds;
}
