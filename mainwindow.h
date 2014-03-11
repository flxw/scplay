#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QMediaPlayer>
# include <QMediaPlaylist>
# include <QNetworkAccessManager>
# include <QNetworkReply>
# include <QSystemTrayIcon>

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
    void startPlaying();
    void processStreamLocationRequestAnswer(QNetworkReply *reply);
    void handleTrayIconActivation(QSystemTrayIcon::ActivationReason activationReason);

private:
    void setupTrayIcon();
    void handleTrayIconSingleClick(void);

private:
    Ui::MainWindow *ui;

    QMediaPlayer* player;
    QMediaPlaylist* playlist;

    QNetworkAccessManager* nwa;
    QString streamLocation;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
