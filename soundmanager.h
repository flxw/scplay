#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

# include <QObject>

# include <QNetworkAccessManager>
# include <QNetworkRequest>
# include <QNetworkReply>

# include <QMediaPlayer>
# include <QMediaPlaylist>

class SoundManager : public QObject
{
    Q_OBJECT
public:
    explicit SoundManager(QObject *parent = 0);

    bool isPlaying();

public slots:
    void play();
    void pause();
    void next();
    void previous();

    void playSound(int id);
    void enqueueSound(int id);

    void handleStreamLocationReply(QNetworkReply* reply);
    void handlePlayerStateChange(QMediaPlayer::State state);

signals:
    void finished(); // emitted when the last song has finished playing
    void nextSongStarted(); // emitted when the next song is begun

    // --- private parts
private:


    // --- attributes ---
public:
private:
    QMediaPlayer*   player;
    QMediaPlaylist* playlist;

    QNetworkAccessManager* networkManager;
    QString streamLocation;
};

#endif // SOUNDMANAGER_H
