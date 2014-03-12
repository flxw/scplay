# include "soundmanager.h"
# include "soundcloudapi.h"

SoundManager::SoundManager(QObject *parent) :
    QObject(parent)
{
    player = new QMediaPlayer(this);

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handlePlayerStateChange(QMediaPlayer::State)));
    connect(&SoundCloudApi::getInstance(), SIGNAL(streamUrlReceived(int,QUrl)), this, SLOT(receiveStreamUrl(int,QUrl)));
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
    SoundCloudApi::getInstance().getStreamUrl(id);
}

void SoundManager::enqueueSound(int id) {
}

void SoundManager::handlePlayerStateChange(QMediaPlayer::State state) {
    switch(state) {
        case QMediaPlayer::StoppedState: emit finished();
        case QMediaPlayer::PlayingState: emit started();
    }
}

void SoundManager::receiveStreamUrl(int id, QUrl url) {
    player->stop();
    player->setMedia(url);
    player->play();
}
