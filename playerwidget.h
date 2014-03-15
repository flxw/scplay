#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

# include <QWidget>
# include <QModelIndex>

# include "soundmanager.h"

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

// === private parts
private: // - methods
    void setupSoundManager();

private slots:
    void handleSliderUserMove();

    void setPlayButtonIcon();
    void setPauseButtonIcon();

private: // - attributes
    SoundManager *soundManager;

    Ui::PlayerWidget *ui;

    QModelIndex currentSongIndex;
    int         currentSongId;
};

#endif // PLAYERWIDGET_H
