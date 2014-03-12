#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QSystemTrayIcon>

# include "soundmanager.h"
# include "soundlistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool event(QEvent* e);

public slots:
    void handleTrayIconActivation(QSystemTrayIcon::ActivationReason activationReason);
    void handlePlayRequest(QModelIndex index);
    void handleNewSongDuration(int d);
    void togglePlayPauseButtonIcon();
    void setPlayButtonIcon();
    void setPauseButtonIcon();

private:
    void setupTrayIcon();
    void setupSoundManager();
    void setupSoundListViews();

    void handleTrayIconSingleClick(void);

    // --- attributes
private:
    SoundManager *soundManager;
    SoundListModel *likeListModel, *playlistModel;

    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
