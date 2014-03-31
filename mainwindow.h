#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QSystemTrayIcon>

# include "soundmodel.h"

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
    void handleAnimationEnd();
    void displayNewSongNotification(QString title,QString user);

    // --- private section ----------------------
private:
    void setupTrayIcon();
    void setupSoundListView();
    void setupWelcomeScreen();
    void setupSoundListViewControls();
    void setupReloadButton();
    void handleTrayIconSingleClick();

private slots:
    void catchFalseLikeListSelectionToggles(bool toggle);
    void catchFalsePlayListSelectionToggles(bool toggle);
    void displaySinglePlaylist(QModelIndex index);

    // --- attributes
private:
    SoundModel *soundModel;

    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
