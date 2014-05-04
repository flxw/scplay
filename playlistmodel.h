#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

# include "listmodelbase.h"
# include "playlist.h"
# include "soundstorage.h"

class PlaylistModel : public ListModelBase
{
    Q_OBJECT

public:
    explicit PlaylistModel(const SoundStorage *storage, QObject *parent = 0);

    Sound getItem(const QModelIndex& index) const;

public slots:
    void updatePlaylistIds(QList<int> newPlaylistIds);
};

#endif // PLAYLISTMODEL_H
