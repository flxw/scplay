#ifndef SOUNDCLOUDAPI_H
#define SOUNDCLOUDAPI_H

# include <QObject>

# include <QNetworkAccessManager>
# include <QNetworkRequest>
# include <QNetworkReply>
# include <QHash>

# include "soundlistitem.h"

class SoundCloudApi : public QObject
{
    Q_OBJECT

public:
    static SoundCloudApi& getInstance();

    void getStreamUrl(int songId);
    void getLikes();

signals:
    void streamUrlReceived(int songId, QUrl streamUrl);
    void likesReceived(QList<SoundListItem> likes);

public slots:

private:
    explicit SoundCloudApi();
    SoundCloudApi(SoundCloudApi const&);   // Don't Implement
    void operator=(SoundCloudApi const&); // Don't implement

private slots:
    void handleFinishedRequest(QNetworkReply *reply);
    void handleStreamUrlReply(QNetworkReply *reply, int id);
    void handleLikeReply(QNetworkReply *reply);

// --- attributes
private:
    QHash<QNetworkReply*, int> waitingStreamUrlReplies;

    QNetworkAccessManager* networkManager;
    QString apiKey;
    int userId;
};
#endif // SOUNDCLOUDAPI_H
