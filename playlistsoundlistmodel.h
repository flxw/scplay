#ifndef PLAYLISTSOUNDLISTMODEL_H
#define PLAYLISTSOUNDLISTMODEL_H

# include "listmodelbase.h"

class PlaylistSoundListModel : public ListModelBase
{
    Q_OBJECT

public:
    explicit PlaylistSoundListModel(const SoundStorage *storage, QObject *parent = 0);

    Sound getItem(const QModelIndex& index) const;

public slots:
    void updateSoundIds(QList<int> newPlaylistIds);
};

#endif // PLAYLISTSOUNDLISTMODEL_H
