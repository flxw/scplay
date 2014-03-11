#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QSystemTrayIcon>

# include "soundmanager.h"

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
    void togglePlayPauseButtonIcon();

private:
    void setupTrayIcon();
    void setupSoundManager();

    void handleTrayIconSingleClick(void);

private:
    SoundManager *soundManager;

    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
