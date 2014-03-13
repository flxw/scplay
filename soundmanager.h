#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

# include <QObject>

# include <QMediaPlayer>
# include <QMediaPlaylist>

class SoundManager : public QObject
{
    Q_OBJECT
public:
    explicit SoundManager(QObject *parent = 0);

    qint64 getDuration();

    bool isPlaying();
    bool isUrlStillValid(const QUrl &url);

    void playUrl(const QUrl& url);

public slots:
    void play();
    void pause();
    void next();
    void previous();

    void requestNewPosition(int p);

    void playSound(int id);
    void enqueueSound(int id);

signals:
    void finished(); // emitted when the last song has finished playing
    void started();
    void nextSongStarted(); // emitted when the next song is begun
    void playTimeElapsed(int duration);
    void newSongDuration(int begin, int duration);

private slots:
    void handlePlayerStateChange(QMediaPlayer::State state);
    void receiveStreamUrl(int id, QUrl url);
    void handleNewDuration(qint64 d);
    void handleNewPosition(qint64 p);

    // --- attributes ---
public:
private:
    QMediaPlayer*   player;
    QMediaPlaylist* playlist;

    QHash<int, QUrl> idsForUrls;

    QString streamLocation;

    int lastRequestedSong;
};

#endif // SOUNDMANAGER_H
