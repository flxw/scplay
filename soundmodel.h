#ifndef SOUNDMODEL_H
#define SOUNDMODEL_H

# include <QAbstractListModel>
# include <QHash>
# include <QList>

# include "sound.h"
# include "playlist.h"

class SoundModel : public QAbstractListModel
{
    Q_OBJECT

    enum FeedState {FEEDING_LIKES, FEEDING_PLAYLISTS, FEEDING_PLAYLIST_SONGS};

public:
    explicit SoundModel(QObject *parent = 0);

    Sound getItem(const QModelIndex& index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role) const;

    int rowCount(const QModelIndex& = QModelIndex()) const;

    void switchToPlaylistSongFeed(const QModelIndex &index);

public slots:
    void updateLikeData(QList<Sound> newLikes);
    void updateArtworkData(int id, QPixmap artwork);
    void updatePlaylists(QList<Sound> sounds, QList<Playlist> playlists);
    void fill();

    // the model "feeds" data to the view, thus we switch the feed states
    void switchToPlaylistFeed();
    void switchToLikeFeed();
    //void switchToPlaylistSongFeed(QModel)

    // --- attributes
protected:
    FeedState state;

    QHash<int, Sound> sounds;
    QHash<int, Playlist> playlists;

    QList<int> likeIds;
    QList<int> playlistIds;

    int currentPlaylistId;
};

#endif // SOUNDMODEL_H
