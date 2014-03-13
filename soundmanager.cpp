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
    disconnect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handlePlayerStateChange(QMediaPlayer::State)));

    player->stop();
    player->setMedia(url);

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handlePlayerStateChange(QMediaPlayer::State)));

    player->play();
}

// --- public slots
void SoundManager::play() {
    player->play();
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

void SoundManager::requestNewPosition(int p) {
    if (player->isSeekable())
        player->setPosition((qint64) p);
    else
        qDebug("not seekable :(");
}

void SoundManager::playSound(int id) {
    lastRequestedSong = id;

    // make url validity check here
    if (idsForUrls.contains(id)) {
        QUrl url = idsForUrls.value(id);
        if (isUrlStillValid(url)) {
            playUrl(url);
            return;
        } else {
            idsForUrls.remove(id);
        }
    }

    SoundCloudApi::getInstance().getStreamUrl(id);
}

void SoundManager::enqueueSound(int id) {
}


// --- private slots
void SoundManager::handlePlayerStateChange(QMediaPlayer::State state) {
    switch(state) {
        case QMediaPlayer::StoppedState: emit finished(); break;
        case QMediaPlayer::PausedState:  emit paused(); break;
        case QMediaPlayer::PlayingState: emit started(); break;
    }
}

void SoundManager::receiveStreamUrl(int id, QUrl url) {
    idsForUrls.insert(id, url);
    playUrl(url);
}

void SoundManager::handleNewDuration(qint64 d) {
    emit newSongDuration(0, (int) d);
}

void SoundManager::handleNewPosition(qint64 p) {
    emit playTimeElapsed((int) p);
}
