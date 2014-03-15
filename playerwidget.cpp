# include "playerwidget.h"
# include "ui_playerwidget.h"

# include "likelistmodel.h"
# include "sounditem.h"

PlayerWidget::PlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerWidget)
{
    ui->setupUi(this);

    // component setup
    setupSoundManager();

    // connections
    connect(ui->prevButton, SIGNAL(clicked()), this, SLOT(playPreviousSong()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(playNextSong()));
}

PlayerWidget::~PlayerWidget()
{
    delete ui;
}

// -- public slots
void PlayerWidget::handlePlayRequest(QModelIndex index) {
    const MyListModel* currentModel = (MyListModel*)index.model();
    const ListItem     song         = currentModel->getItem(index);

    // should prev/next buttons be enabled?
    ui->prevButton->setEnabled(index.row() > 0);
    ui->nextButton->setEnabled(index.row() < (currentModel->rowCount() - 1));

    soundManager->playSound(song.getId());

    ui->soundNameLabel->setText(song.getTitle());
    ui->creatorNameLabel->setText(song.getUser());

    // handle artwork display here

    currentSongIndex = index;
}

void PlayerWidget::togglePlayPause() {
    soundManager->pause();
}

void PlayerWidget::playNextSong() {
    QModelIndex nextSongIndex = currentSongIndex.model()->index(currentSongIndex.row()+1,currentSongIndex.column());

    if (nextSongIndex.isValid()) {
        handlePlayRequest(nextSongIndex);
    } else {
        ui->nextButton->setEnabled(false);
        setPlayButtonIcon();
    }
}

void PlayerWidget::playPreviousSong() {
    QModelIndex prevSongIndex = currentSongIndex.model()->index(currentSongIndex.row()-1,currentSongIndex.column());

    if (prevSongIndex.isValid()) {
        handlePlayRequest(prevSongIndex);
    } else {
        ui->prevButton->setEnabled(false);
        setPlayButtonIcon();
    }
}
// -- private slots
void PlayerWidget::handleSliderUserMove() {
    soundManager->requestNewPosition(ui->progressBar->sliderPosition());
}

void PlayerWidget::setPlayButtonIcon() {
    ui->playPauseButton->setText(" \u25B6");

    disconnect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(pause()));
    connect(ui->playPauseButton,    SIGNAL(clicked()), soundManager, SLOT(play()));
}

void PlayerWidget::setPauseButtonIcon() {
    ui->playPauseButton->setText("\u007C\u007C");

    disconnect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(play()));
    connect(ui->playPauseButton,    SIGNAL(clicked()), soundManager, SLOT(pause()));
}

// -- private methods
void PlayerWidget::setupSoundManager() {
    soundManager = new SoundManager(this);

    connect(soundManager, SIGNAL(started()),  this, SLOT(setPauseButtonIcon()));
    connect(soundManager, SIGNAL(paused()),   this, SLOT(setPlayButtonIcon()));
    connect(soundManager, SIGNAL(finished()), this, SLOT(playNextSong()));

    connect(soundManager,    SIGNAL(newSongDuration(int, int)), ui->progressBar, SLOT(setRange(int,int)));
    connect(soundManager,    SIGNAL(playTimeElapsed(int)),      ui->progressBar, SLOT(setValue(int)));
    connect(ui->progressBar, SIGNAL(sliderReleased()),          this,            SLOT(handleSliderUserMove()));

    // this is being connected to the pause slot, as the button is only enabled upon play beging
    connect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(pause()));
}
