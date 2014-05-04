#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QSystemTrayIcon>

# include "likemodel.h"
# include "playlistmodel.h"
# include "playlistsoundlistmodel.h"
# include "soundstorage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void focusOutEvent(QFocusEvent *e);

public slots:
    void handleTrayIconActivation(QSystemTrayIcon::ActivationReason activationReason);
    void displayNewSongNotification(QString title,QString user);

    void switchToPlaylistListingDisplay();
    void switchToLikeDisplay();

    void selectPlaylist(QModelIndex index);

    // --- private section ----------------------
private:
    void setupTrayIcon();
    void setupModels();
    void setupSoundListView();
    void setupWelcomeScreen();
    void setupReloadButton();
    void handleTrayIconSingleClick();


    // --- attributes
private:
    LikeModel *likeModel;
    PlaylistModel *playlistModel;
    SoundStorage *soundStorage;

    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
