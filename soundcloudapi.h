#ifndef SOUNDCLOUDAPI_H
#define SOUNDCLOUDAPI_H

# include <QObject>

# include <QNetworkAccessManager>
# include <QNetworkRequest>
# include <QNetworkReply>
# include <QHash>

# include "sounditem.h"

class SoundCloudApi : public QObject
{
    Q_OBJECT

public:
    static SoundCloudApi& getInstance();

    void getStreamUrl(int songId);
    void getLikes();
    void getArtwork(int songId, QUrl artworkUrl);

public slots:
    void setUserPermaLink(QString name);
    void setUserId(int userId);

signals:
    void streamUrlReceived(int songId, QUrl streamUrl);
    void likesReceived(QList<SoundItem> likes);
    void artworkReceived(int songId, QPixmap& artwork);
    void isReady(); // emitted when a valid profile id has been set
    void badUserIdGiven();


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

// --- attributes
private:
    QHash<QNetworkReply*, int> waitingStreamUrlReplies;
    QHash<QNetworkReply*, int> waitingArtworkReplies;
    QList<QNetworkReply*>      waitingLikeReplies;
    QNetworkReply*             waitingUserIdReply;

    QNetworkAccessManager* networkManager;

    int userId;
};
#endif // SOUNDCLOUDAPI_H
