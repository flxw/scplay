#ifndef DATASTORE_H
#define DATASTORE_H

# include <QObject>
# include <QList>
# include <QHash>

# include "sounditem.h"

class DataStore : public QObject
{
    Q_OBJECT
public:
    static DataStore& getInstance();

    SoundItem getSound(int id) const;
    // ListItem get

signals:
    void likesUpdated(QList<int> likes);
    // void playlistsUpdate(QList<int> playlists);

public slots:
    void initiallyFillStore();

    void updateLikes(QList<SoundItem> newLikes);
    //void updatePlaylists(QList<PlaylistItem> newPlaylists, QList<SoundItem> containedSongs);

// === privates ==========================
private:
    explicit DataStore();
    DataStore(DataStore const&);   // Don't Implement
    void operator=(DataStore const&); // Don't implement

private:
    QHash<int, SoundItem> songs;
};

#endif // DATASTORE_H
