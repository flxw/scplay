# include "mainwindow.h"
# include "ui_mainwindow.h"

# include <QMenu>
# include <QPropertyAnimation>

# include "likelistmodel.h"
# include "soundcloudapi.h"
# include "enterusernamewidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent, Qt::FramelessWindowHint), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->hide();

    // --- other setups
    setupSoundManager();
    setupTrayIcon();
    setupSoundListViews();

#ifndef QT_DEBUG
    setupWelcomeScreen();
#endif

    // -- connections
    connect(ui->prevButton, SIGNAL(clicked()), this, SLOT(playPreviousSong()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(playNextSong()));

    connect(&SoundCloudApi::getInstance(), SIGNAL(isReady()), likeListModel, SLOT(fillModel()));
    //connect(&SoundCloudApi::getInstance(), SIGNAL(isReady()), playListModel, SLOT(fillModel()));

#ifdef QT_DEBUG
    SoundCloudApi::getInstance().setUserId(62853215);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- public functions
bool MainWindow::event(QEvent* e)
{
    switch(e->type()) {
    case QEvent::WindowActivate :
        // gained focus
        break;

    case QEvent::WindowDeactivate :
#ifndef QT_DEBUG
        this->hide();
#endif
        break;

    default: break;
    }

    return QMainWindow::event(e) ;
}

// --- public slots
void MainWindow::handleTrayIconActivation(QSystemTrayIcon::ActivationReason activationReason) {
    switch (activationReason) {
        case QSystemTrayIcon::Trigger:
            handleTrayIconSingleClick();
            break;

        case QSystemTrayIcon::Context:
            break;

        default:
            qDebug("Unhandled tray icon activation: %i", (int)activationReason);
            break;
    }
}

void MainWindow::handlePlayRequest(QModelIndex index) {
    ui->playPauseButton->setEnabled(true);

    const SoundListModel* currentModel = (SoundListModel*)index.model();
    const SoundListItem&  soundItem    = currentModel->getSongItem(index);

    // should prev/next buttons be enabled?
    ui->prevButton->setEnabled(index.row() > 0);
    ui->nextButton->setEnabled(index.row() < (currentModel->rowCount() - 1));

    soundManager->playSound(soundItem.getId());

    ui->soundNameLabel->setText(soundItem.getTitle());
    ui->creatorNameLabel->setText(soundItem.getUser());

    currentSongIndex = index;
}

void MainWindow::handleSliderUserMove() {
    soundManager->requestNewPosition(ui->progressBar->sliderPosition());
}

void MainWindow::playNextSong() {
    QModelIndex nextSongIndex = currentSongIndex.model()->index(currentSongIndex.row()+1,currentSongIndex.column());

    if (nextSongIndex.isValid()) {
        handlePlayRequest(nextSongIndex);
    } else {
        ui->nextButton->setEnabled(false);
        setPlayButtonIcon();
    }
}

void MainWindow::playPreviousSong() {
    QModelIndex prevSongIndex = currentSongIndex.model()->index(currentSongIndex.row()-1,currentSongIndex.column());

    if (prevSongIndex.isValid()) {
        handlePlayRequest(prevSongIndex);
    } else {
        ui->prevButton->setEnabled(false);
        setPlayButtonIcon();
    }
}

void MainWindow::setPlayButtonIcon() {
    ui->playPauseButton->setText(" \u25B6");

    disconnect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(pause()));
    connect(ui->playPauseButton,    SIGNAL(clicked()), soundManager, SLOT(play()));
}

void MainWindow::setPauseButtonIcon() {
    ui->playPauseButton->setText("\u007C\u007C");

    disconnect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(play()));
    connect(ui->playPauseButton,    SIGNAL(clicked()), soundManager, SLOT(pause()));
}

// --- private functions
void MainWindow::setupTrayIcon() {
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/white.png"), this);

    QMenu* trayMenu = new QMenu();

    trayMenu->addAction("\u25B6 / \u2016", ui->playPauseButton, SIGNAL(clicked()));
    trayMenu->addAction("\u25B6\u25B6\u007C", soundManager, SLOT(next()));
    trayMenu->addAction("\u007C\u25C0\u25C0", soundManager, SLOT(previous()));
    trayMenu->addAction("Exit", this, SLOT(close()));

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(handleTrayIconActivation(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::setupSoundManager() {
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

void MainWindow::handleTrayIconSingleClick() {
    QPoint cursorPosition = QCursor::pos();

    // move() moves the left-upper corner of the window to the specified position
    this->move(cursorPosition.x() - width(), cursorPosition.y() + 20);
    this->show();
}

void MainWindow::setupSoundListViews() {
    likeListModel = new LikeListModel(this);

    ui->songView->setModel(likeListModel);
    connect(ui->songView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(handlePlayRequest(QModelIndex)));
}

void MainWindow::setupWelcomeScreen() {
    QFrame*  helloUserFrame  = new QFrame(this);
    QWidget* helloUserScreen = new EnterUserNameWidget(helloUserFrame);

    QPropertyAnimation *slideOutAnimation = new QPropertyAnimation(helloUserFrame, "geometry");

    slideOutAnimation->setDuration(500);
    slideOutAnimation->setStartValue(QRect(0,0, 300, 400));
    slideOutAnimation->setEndValue(QRect(-300, 0, 300, 400));

    helloUserFrame->setMinimumSize(this->size());
    helloUserFrame->setStyleSheet("background-color: #333;");

    connect(&SoundCloudApi::getInstance(), SIGNAL(isReady()), slideOutAnimation, SLOT(start()));
}
