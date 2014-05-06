#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QSystemTrayIcon>

# include "likemodel.h"
# include "soundstorage.h"
# include "playlistmodel.h"
# include "activitylistmodel.h"

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
    void displayNewSongNotification(QString title,QString user);

    void switchToPlaylistListingDisplay();
    void switchToLikeDisplay();
    void switchToActivityDisplay();

    void selectPlaylist(QModelIndex index);

    // --- private section ----------------------
private:
    void setupTrayIcon();
    void setupModels();
    void setupSoundListView();
    void setupWelcomeScreen();
    void setupReloadButton();
    void handleTrayIconSingleClick();

    // --- attributes ---------------------------
private:
    LikeModel         *likeModel;
    PlaylistModel     *playlistModel;
    SoundStorage      *soundStorage;
    ActivityListModel *activityModel;

    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
