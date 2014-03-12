# include "soundmanager.h"
# include "soundcloudapi.h"

SoundManager::SoundManager(QObject *parent) :
    QObject(parent)
{
    player = new QMediaPlayer(this);

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handlePlayerStateChange(QMediaPlayer::State)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(handleNewDuration(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(handleNewPosition(qint64)));
    connect(&SoundCloudApi::getInstance(), SIGNAL(streamUrlReceived(int,QUrl)), this, SLOT(receiveStreamUrl(int,QUrl)));
}


// --- public functions
bool SoundManager::isPlaying() {
    return (player->state() == QMediaPlayer::PlayingState);
}

bool SoundManager::isUrlStillValid(const QUrl &url) {
    QString queryString = url.toString();
    int expireStringIndex = queryString.indexOf("Expires=") + 8;
    QString timeString = queryString.mid(expireStringIndex, 10);

    qint64 exp_msecs = timeString.toInt();
    qint64 cur_msecs = QDateTime::currentMSecsSinceEpoch();

    if (cur_msecs < exp_msecs) return true;
    else return false;
}

void SoundManager::playUrl(const QUrl &url) {
    player->stop();
    player->setMedia(url);
    player->play();
}

// --- public slots
void SoundManager::play() {
    // stub - to be removed
    if (player->state() == QMediaPlayer::StoppedState) playSound(98800029); // 2,147,483,647
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
    lastRequestedSong = id;

    if (idsForUrls.contains(id)) {
        QUrl url = idsForUrls.value(id);
        if (isUrlStillValid(url)) {
            playUrl(url);
            return;
        }
    }

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
    idsForUrls.insert(id, url);
    playUrl(url);
}

void SoundManager::handleNewDuration(qint64 d) {
    emit newSongDuration((int) d/1000);
}

void SoundManager::handleNewPosition(qint64 p) {
    emit playTimeElapsed((int) p/1000);
}
