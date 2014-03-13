# include "soundcloudapi.h"

# include <QJsonDocument>
# include <QJsonArray>
# include <QJsonObject>

# define API_KEY "798808c14d25fc803a4f484e821ca63a"

// --- public methods
SoundCloudApi& SoundCloudApi::getInstance() {
    static SoundCloudApi instance; // Guaranteed to be destroyed.
                           // Instantiated on first use.
    return instance;
}

void SoundCloudApi::getStreamUrl(int songId) {
    static QString urlTemplate("http://api.sndcdn.com/i1/tracks/%1/streams?client_id=" API_KEY);

    QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(songId))));

    waitingStreamUrlReplies.insert(reply, songId);
}

void SoundCloudApi::getLikes() {
    static QString urlTemplate("http://api.soundcloud.com/users/%1/favorites.json?client_id=" API_KEY);

    networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(userId))));
}

/* resolves the soundcloud username to the internal user id */
void SoundCloudApi::setUserPermaLink(QString name) {
    static QString urlTemplate("http://api.soundcloud.com/resolve.json?url=http://soundcloud.com/%1&client_id=" API_KEY);

    waitingUserIdReply = networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(userId))));
}

void SoundCloudApi::setUserId(int userId) {
    this->userId = userId;
    emit isReady();
}


// --- private methods
SoundCloudApi::SoundCloudApi() {
    userId = -1;
    waitingUserIdReply = NULL;

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleFinishedRequest(QNetworkReply*)));
}

// --- private slots
void SoundCloudApi::handleFinishedRequest(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        if (waitingStreamUrlReplies.contains(reply)) {
            handleStreamUrlReply(reply, waitingStreamUrlReplies.take(reply));
        } else if (reply == waitingUserIdReply) {
            handleUserIdReply(reply);
        } else {
            handleLikeReply(reply);
        }
    }
}

void SoundCloudApi::handleStreamUrlReply(QNetworkReply *reply, int id) {
    QString replyString = QString(reply->readAll());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyString.toUtf8());

    QJsonValue streamUrl = jsonDocument.object()["http_mp3_128_url"];

    if (streamUrl.isString()) {
        emit streamUrlReceived(id, QUrl(streamUrl.toString()));
    }
}

void SoundCloudApi::handleLikeReply(QNetworkReply *reply) {
    QString replyString = QString(reply->readAll());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyString.toUtf8());
    QJsonArray  jsonArray = jsonDocument.array();
    QJsonObject songObject;

    QList<SoundListItem> soundItems;

    for (QJsonArray::const_iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
        SoundListItem listItem;

        songObject = (*it).toObject();
        listItem.setId(songObject["id"].toInt());
        listItem.setTitle(songObject["title"].toString());
        listItem.setUser(songObject["user"].toObject()["username"].toString());

        soundItems.append(listItem);
    }

    emit likesReceived(soundItems);
}

void SoundCloudApi::handleUserIdReply(QNetworkReply* reply) {
    waitingUserIdReply  = NULL;
    QString replyString = reply->readAll();

    bool ok = false;
    int id = replyString.mid(32, (replyString.indexOf(".json") - 33)).toInt(&ok);

    if (ok && id > 0) {
        userId = id;
        emit isReady();
    }
}
