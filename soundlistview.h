#ifndef SOUNDLISTVIEW_H
#define SOUNDLISTVIEW_H

# include <QListView>

# include "likemodel.h"
# include "playlistmodel.h"
# include "playlistsoundlistmodel.h"
# include "activitylistmodel.h"
# include "soundstorage.h"

class SoundListView : public QListView
{
    Q_OBJECT
public:
    explicit SoundListView(QWidget *parent = 0);

signals:
    void soundSelected(QModelIndex soundIndex);

public slots:
    void switchToPlaylistListingDisplay();
    void switchToLikeDisplay();
    void switchToActivityDisplay();

    void refresh();

    // --- private section ----------------------
private:
    void setupModels();

private slots:
    void selectPlaylist(QModelIndex playlistIndex);
    void selectActivity(QModelIndex activityIndex);

    // --- attributes ---------------------------
private:
    SoundStorage*      soundStorage;
    PlaylistModel*     playlistModel;
    LikeModel*         likeModel;
    ActivityListModel* activityModel;
};

#endif // SOUNDLISTVIEW_H
