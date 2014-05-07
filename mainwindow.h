#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QSystemTrayIcon>

# include "soundlistview.h"

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

    // --- private section ----------------------
private:
    void setupTrayIcon();
    void setupAnimation();
    void setupWelcomeScreen();
    void setupReloadButton();
    void setupSoundList();

    void handleTrayIconSingleClick();

    // --- attributes ---------------------------
private:
    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
