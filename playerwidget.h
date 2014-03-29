#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

# include <QWidget>
# include <QModelIndex>

# include "playbackmanager.h"

namespace Ui {
class PlayerWidget;
}

class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = 0);
    ~PlayerWidget();

public slots:
    void handlePlayRequest(QModelIndex index);
    void togglePlayPause();
    void playNextSong();
    void playPreviousSong();
    void handleArtworkUpdate(int id, QPixmap& p);

signals:
    void playbackStarted();

// === private parts
private: // - methods
    void setupPlaybackManager();

private slots:
    void handleSliderUserMove();

    void setPlayButtonIcon();
    void setPauseButtonIcon();

private: // - attributes
    PlaybackManager *soundManager;

    Ui::PlayerWidget *ui;

    QModelIndex currentSongIndex;
    int         currentSongId;
};

#endif // PLAYERWIDGET_H
