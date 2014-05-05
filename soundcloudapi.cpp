# include "soundcloudapi.h"

# include <QJsonDocument>
# include <QJsonArray>
# include <QJsonObject>
# include <QDesktopServices>

# define CLIENT_ID "23896355641020e5f7e2292321372280"
# define CLIENT_SECRET "0a49f062eaaa04561cc4f74d42da6739"
# define OAUTH_LOCAL_SERVER_PORT 9999

// --- public methods
SoundCloudApi& SoundCloudApi::getInstance() {
    static SoundCloudApi instance; // Guaranteed to be destroyed.
                           // Instantiated on first use.
    return instance;
}

bool SoundCloudApi::isAuthenticated() {
    return oauthAuthenticator->linked();
}

void SoundCloudApi::requestStreamUrl(int songId) {
    static QString urlTemplate("http://api.sndcdn.com/i1/tracks/%1/streams?client_id=" CLIENT_ID);

    QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(songId))));

    waitingStreamUrlReplies.insert(reply, songId);
}

void SoundCloudApi::requestLikes() {
    static QString urlTemplate("https://api.soundcloud.com/me/favorites.json?oauth_token=%1");

    QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(oauthAuthenticator->token()))));

    waitingLikeReplies.append(reply);
}

void SoundCloudApi::requestArtwork(int songId, QUrl artworkUrl) {
    QNetworkReply* reply = networkManager->get(QNetworkRequest(artworkUrl));

    waitingArtworkReplies.insert(reply, songId);
}

void SoundCloudApi::requestPlaylists() {
    static QString urlTemplate("https://api.soundcloud.com/me/playlists.json?oauth_token=%1");

    QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(oauthAuthenticator->token()))));

    waitingPlaylistReplies.append(reply);
}

void SoundCloudApi::requestAuthentification() {
    oauthAuthenticator->setGrantFlow(O2::GrantFlowAuthorizationCode);
    oauthAuthenticator->link();
}
// --- private methods
SoundCloudApi::SoundCloudApi() {
    networkManager = new QNetworkAccessManager(this);

    oauthAuthenticator = new O2(this);
    oauthRequestor     = new O2Requestor(networkManager, oauthAuthenticator, this);

    oauthAuthenticator->setClientId(CLIENT_ID);
    oauthAuthenticator->setClientSecret(CLIENT_SECRET);
    oauthAuthenticator->setScope("non-expiring");
    oauthAuthenticator->setLocalPort(OAUTH_LOCAL_SERVER_PORT);
    oauthAuthenticator->setRequestUrl("https://soundcloud.com/connect");
    oauthAuthenticator->setTokenUrl("https://api.soundcloud.com/oauth2/token");

    // sum connecthunz
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleFinishedRequest(QNetworkReply*)));

    connect(oauthAuthenticator, SIGNAL(linkedChanged()),    this, SLOT(onOauthLinkedChanged()));
    connect(oauthAuthenticator, SIGNAL(linkingFailed()),    this, SLOT(onOauthLinkingFailed()));
    connect(oauthAuthenticator, SIGNAL(linkingSucceeded()), this, SLOT(onOauthLinkingSucceeded()));
    connect(oauthAuthenticator, SIGNAL(openBrowser(QUrl)),  this, SLOT(onOauthOpenBrowser(QUrl)));
}

// --- private slots
void SoundCloudApi::handleFinishedRequest(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        if (waitingStreamUrlReplies.contains(reply)) {
            handleStreamUrlReply(reply);
        } else if (waitingLikeReplies.contains(reply)) {
            handleLikeReply(reply);
        } else if (waitingArtworkReplies.contains(reply)) {
            handleArtworkReply(reply);
        } else if (waitingPlaylistReplies.contains(reply)) {
            handlePlaylistReply(reply);
        }
    } else {
        qDebug("Network error %i | %s", (int)reply->error(), reply->url().toString().toStdString().c_str());

        if (waitingStreamUrlReplies.contains(reply)) {
            waitingStreamUrlReplies.remove(reply);
            // emit badStreamUrlRequest?!
        } else if (waitingLikeReplies.contains(reply)){
            waitingLikeReplies.removeOne(reply);
            // emit badLikeRequest?!
        } else if (waitingPlaylistReplies.contains(reply)) {
            // emit badPlaylistRequest?!
        }
    }
}

void SoundCloudApi::handleStreamUrlReply(QNetworkReply *reply) {
    QString replyString = QString(reply->readAll());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyString.toUtf8());

    QJsonValue streamUrl = jsonDocument.object()["http_mp3_128_url"];

    if (streamUrl.isString()) {
        emit streamUrlReceived(waitingStreamUrlReplies.take(reply), QUrl(streamUrl.toString()));
    }
}

void SoundCloudApi::handleLikeReply(QNetworkReply *reply) {
    QString replyString = QString(reply->readAll());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyString.toUtf8());
    QJsonArray  jsonArray = jsonDocument.array();
    QJsonObject songObject;

    QList<Sound> likes;

    for (QJsonArray::const_iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
        songObject = (*it).toObject();

        Sound like;

        like.setId(songObject["id"].toInt());
        like.setTitle(songObject["title"].toString());
        like.setUser(songObject["user"].toObject()["username"].toString());
        like.setArtworkUrl(songObject["artwork_url"].toString());

        likes.append(like);
    }

    waitingLikeReplies.removeOne(reply);

    emit likesReceived(likes);
}

void SoundCloudApi::handleArtworkReply(QNetworkReply *reply) {
    QPixmap p;
    QByteArray picBytes = reply->readAll();
    int id = waitingArtworkReplies.take(reply);

    if (p.loadFromData(picBytes, "JPG")) {
        emit artworkReceived(id, p);
    }
}

void SoundCloudApi::handlePlaylistReply(QNetworkReply *reply) {
    QString replyString = QString(reply->readAll());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyString.toUtf8());
    QJsonArray  jsonArray = jsonDocument.array();

    QList<Sound> sounds;
    QList<Playlist> playlists;

    for (QJsonArray::const_iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
        QJsonObject playlistJson = (*it).toObject();

        QList<int> playlistSounds;

        // read all tracks from this playlist
        QJsonArray playlistSoundsJson = playlistJson["tracks"].toArray();

        for (QJsonArray::const_iterator psit = playlistSoundsJson.begin(); psit != playlistSoundsJson.end(); ++psit) {
            QJsonObject soundJson = (*psit).toObject();

            Sound sound;

            sound.setId(soundJson["id"].toInt());
            sound.setTitle(soundJson["title"].toString());
            sound.setUser(soundJson["user"].toObject()["username"].toString());
            sound.setArtworkUrl(soundJson["artwork_url"].toString());

            sounds.append(sound);
            playlistSounds.append(sound.getId());
        }

        // provide some information about this playlist
        Playlist playlist(playlistSounds);

        playlist.setId(playlistJson["id"].toInt());
        playlist.setUser(playlistJson["user"].toObject()["username"].toString());
        playlist.setTitle(playlistJson["title"].toString());
        playlist.setArtworkUrl(playlistJson["artwork_url"].toString());

        playlists.append(playlist);
    }

    waitingPlaylistReplies.removeOne(reply);

    emit playlistsReceived(sounds, playlists);
}

void SoundCloudApi::onOauthLinkedChanged() {
    if (oauthAuthenticator->linked()) {
        emit authenticated();
    } else {
        emit notAuthenticated();
    }
}

void SoundCloudApi::onOauthLinkingFailed() {
    // Login has failed
    emit notAuthenticated();
}

void SoundCloudApi::onOauthLinkingSucceeded() {
    // Login has succeeded
    qDebug() << "Gained auth like a boss!";
    emit isAuthenticated();
}

void SoundCloudApi::onOauthOpenBrowser(const QUrl url) {
    QDesktopServices::openUrl(url);
}
