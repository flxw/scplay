#ifndef SOUNDCLOUDAPI_H
#define SOUNDCLOUDAPI_H

# include <QObject>

# include <QNetworkAccessManager>
# include <QNetworkRequest>
# include <QNetworkReply>
# include <QHash>

# include "sound.h"
# include "playlist.h"

class SoundCloudApi : public QObject
{
    Q_OBJECT

public:
    static SoundCloudApi& getInstance();

    void requestStreamUrl(int songId);
    void requestLikes();
    void requestArtwork(int songId, QUrl artworkUrl);
    void requestPlaylists();

    int getUserId() const;

public slots:
    void setUserPermaLink(QString name);
    void setUserId(int userId);

signals:
    void streamUrlReceived(int songId, QUrl streamUrl);
    void likesReceived(QList<Sound> likes);
    void artworkReceived(int songId, QPixmap& artwork);
    void isReady(); // emitted when a valid profile id has been set
    void badUserIdGiven();
    void playlistsReceived(QList<Sound> sounds, QList<Playlist> playlists);


// === privates =================================
private:
    explicit SoundCloudApi();
    SoundCloudApi(SoundCloudApi const&);   // Don't Implement
    void operator=(SoundCloudApi const&); // Don't implement

private slots:
    void handleFinishedRequest(QNetworkReply *reply);
    void handleStreamUrlReply(QNetworkReply *reply);
    void handleLikeReply(QNetworkReply *reply);
    void handleUserIdReply(QNetworkReply *reply);
    void handleArtworkReply(QNetworkReply *reply);
    void handlePlaylistReply(QNetworkReply *reply);

// --- attributes
private:
    QHash<QNetworkReply*, int> waitingStreamUrlReplies;
    QHash<QNetworkReply*, int> waitingArtworkReplies;
    QList<QNetworkReply*>      waitingLikeReplies;
    QList<QNetworkReply*>      waitingPlaylistReplies;
    QNetworkReply*             waitingUserIdReply;

    QNetworkAccessManager* networkManager;

    int userId;
};
#endif // SOUNDCLOUDAPI_H
