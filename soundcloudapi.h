#ifndef SOUNDCLOUDAPI_H
#define SOUNDCLOUDAPI_H

# include <QObject>

# include <QNetworkAccessManager>
# include <QNetworkRequest>
# include <QNetworkReply>
# include <QHash>

# include "o2.h"
# include "o2requestor.h"

# include "sound.h"
# include "playlist.h"

class SoundCloudApi : public QObject
{
    Q_OBJECT

public:
    static SoundCloudApi& getInstance();

    bool isAuthenticated();

    void requestStreamUrl(int songId);
    void requestLikes();
    void requestArtwork(int songId, QUrl artworkUrl);
    void requestPlaylists();
    void requestAuthentification();

signals:
    void streamUrlReceived(int songId, QUrl streamUrl);
    void likesReceived(QList<Sound> likes);
    void artworkReceived(int songId, QPixmap& artwork);
    void authenticated(); // emitted when a valid profile id has been set
    void notAuthenticated();
    void playlistsReceived(QList<Sound> sounds, QList<Playlist> playlists);

// === privates =================================
private:
    explicit SoundCloudApi();
    SoundCloudApi(SoundCloudApi const&);   // Don't Implement
    void operator=(SoundCloudApi const&);// Don't implement

private slots:
    void handleFinishedRequest(QNetworkReply *reply);
    void handleStreamUrlReply(QNetworkReply *reply);
    void handleLikeReply(QNetworkReply *reply);
    void handleArtworkReply(QNetworkReply *reply);
    void handlePlaylistReply(QNetworkReply *reply);

    void onOauthLinkedChanged();
    void onOauthLinkingFailed();
    void onOauthLinkingSucceeded();
    void onOauthOpenBrowser(const QUrl url);

// --- attributes
private:
    QHash<QNetworkReply*, int> waitingStreamUrlReplies;
    QHash<QNetworkReply*, int> waitingArtworkReplies;
    QList<QNetworkReply*>      waitingLikeReplies;
    QList<QNetworkReply*>      waitingPlaylistReplies;

    QNetworkAccessManager* networkManager;

    O2          *oauthAuthenticator;
    O2Requestor *oauthRequestor;
};
#endif // SOUNDCLOUDAPI_H
