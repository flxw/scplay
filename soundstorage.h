#ifndef SOUNDSTORAGE_H
#define SOUNDSTORAGE_H

# include <QObject>
# include <QList>
# include <QHash>

# include "sound.h"
# include "playlist.h"

class SoundStorage : public QObject
{
    Q_OBJECT
public:
    explicit SoundStorage(QObject *parent = 0);

    Sound getSoundById(int id) const;
    Sound getPlaylistById(int id) const;

signals:
    void playlistsUpdated(QList<int> newPlaylistIds);
    void likesUpdated(QList<int> newLikeIds);

public slots:
    void fill();

private slots:
    void updateLikes(QList<Sound> newLikes);
    void updatePlaylists(QList<Sound> containedSongs, QList<Playlist> newPlaylists);

// --- attributes
private:
    QHash<int, Sound> sounds;
    QHash<int, Playlist> playlists;
};

#endif // SOUNDSTORAGE_H
