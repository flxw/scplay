# include "soundlistview.h"

# include "soundlistdelegate.h"

SoundListView::SoundListView(QWidget *parent) : QListView(parent) {
    setItemDelegate(new SoundListDelegate(this));

    setupModels();

    switchToActivityDisplay();
}

// --- public slots -----------------------------
void SoundListView::switchToPlaylistListingDisplay() {
    disconnect(this, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(soundSelected(QModelIndex)));
    disconnect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectPlaylist(QModelIndex)));

    this->setModel(playlistModel);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectPlaylist(QModelIndex)));
}

void SoundListView::switchToLikeDisplay() {
    disconnect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectActivity(QModelIndex)));
    disconnect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectPlaylist(QModelIndex)));

    this->setModel(likeModel);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(soundSelected(QModelIndex)));
}

void SoundListView::switchToActivityDisplay() {
    disconnect(this, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(soundSelected(QModelIndex)));
    disconnect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectPlaylist(QModelIndex)));

    this->setModel(activityModel);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectActivity(QModelIndex)));
}

void SoundListView::refresh() {
    soundStorage->fill();
}

// --- private methods --------------------------
void SoundListView::setupModels() {
    soundStorage  = new SoundStorage(this);
    likeModel     = new LikeModel(soundStorage, this);
    playlistModel = new PlaylistModel(soundStorage, this);
    activityModel = new ActivityListModel(soundStorage, this);
}

// --- private slots ----------------------------
void SoundListView::selectPlaylist(QModelIndex index) {
    // introducing a possible memory leak here....
    PlaylistSoundListModel *playlistSoundModel = new PlaylistSoundListModel(soundStorage, this);
    int selectedPlaylistId = ((ListModelBase*)index.model())->getItem(index).getId();
    Playlist selectedPlaylist = soundStorage->getPlaylistById(selectedPlaylistId);

    playlistSoundModel->updateSoundIds(selectedPlaylist.getSounds());
    this->setModel(playlistSoundModel);

    disconnect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectPlaylist(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(soundSelected(QModelIndex)));
}

void SoundListView::selectActivity(QModelIndex activityIndex) {
    if (activityModel->isPlaylistSelected(activityIndex.row())) {
        disconnect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectActivity(QModelIndex)));

        selectPlaylist(activityIndex);
    } else {
        emit soundSelected(activityIndex);
    }
}
