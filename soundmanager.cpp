# include "soundmanager.h"

# include <QJsonDocument>
# include <QJsonObject>
# include <QJsonValue>

SoundManager::SoundManager(QObject *parent) :
    QObject(parent)
{
    player = new QMediaPlayer(this);
    networkManager = new QNetworkAccessManager(this);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleStreamLocationReply(QNetworkReply*)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handlePlayerStateChange(QMediaPlayer::State)));
}


// --- public functions
bool SoundManager::isPlaying() {
    return (player->state() == QMediaPlayer::PlayingState);
}

// --- public slots
void SoundManager::play() {
    // stub - to be removed
    if (player->state() == QMediaPlayer::StoppedState) playSound(98800029);
    else player->play();
}

void SoundManager::pause() {
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else if (player->state() == QMediaPlayer::PausedState) {
        player->play();
    }
}

void SoundManager::next() {
    // TODO
}

void SoundManager::previous() {
    // TODO
}

void SoundManager::playSound(int id) {
    static QString urlTemplate("http://api.sndcdn.com/i1/tracks/%1/streams?client_id=798808c14d25fc803a4f484e821ca63a");

    networkManager->get(QNetworkRequest(QUrl(urlTemplate.arg(id))));
}

void SoundManager::enqueueSound(int id) {

}

void SoundManager::handleStreamLocationReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString replyString = QString(reply->readAll());
        QJsonDocument jsonDocument = QJsonDocument::fromJson(replyString.toUtf8());

        QJsonValue streamUrl = jsonDocument.object()["http_mp3_128_url"];

        if (streamUrl.isString()) {
            player->stop();
            player->setMedia(QUrl(streamUrl.toString()));
            player->play();
        }
    }
}

void SoundManager::handlePlayerStateChange(QMediaPlayer::State state) {
    if (state == QMediaPlayer::StoppedState) emit finished();
}
