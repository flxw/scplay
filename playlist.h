#ifndef PLAYLIST_H
#define PLAYLIST_H

# include "sound.h"

class Playlist : public Sound
{
public:
    Playlist(QList<int>& sounds);

    const QList<int>& getSounds() const;

private:
    QList<int> soundIds;
};

#endif // PLAYLIST_H
