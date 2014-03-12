# include "soundcloudapi.h"

# include <QJsonDocument>
# include <QJsonArray>
# include <QJsonObject>

// --- public methods
SoundCloudApi& SoundCloudApi::getInstance() {
    static SoundCloudApi instance; // Guaranteed to be destroyed.
                           // Instantiated on first use.
    return instance;
}

void SoundCloudApi::getStreamUrl(int songId) {
    static QString urlTemplate("http://api.sndcdn.com/i1/tracks/%1/streams?client_id=%2");

    QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(songId).arg(apiKey))));

    waitingStreamUrlReplies.insert(reply, songId);
}

void SoundCloudApi::getLikes() {
    static QString urlTemplate("http://api.soundcloud.com/users/%1/favorites.json?client_id=%2");

    networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(userId).arg(apiKey))));
}


// --- private methods
SoundCloudApi::SoundCloudApi() {
    this->apiKey = "798808c14d25fc803a4f484e821ca63a";
    this->userId = 62853215;

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleFinishedRequest(QNetworkReply*)));
}

// --- private slots
void SoundCloudApi::handleFinishedRequest(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        if (waitingStreamUrlReplies.contains(reply)) {
            handleStreamUrlReply(reply, waitingStreamUrlReplies.take(reply));
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
