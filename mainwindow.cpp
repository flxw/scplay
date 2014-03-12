# include "mainwindow.h"
# include "ui_mainwindow.h"

# include <QMenu>

# include <likelistmodel.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->hide();

    // --- other setups
    setupSoundManager();
    setupTrayIcon();
    setupSoundListViews();

    // --- connections
    connect(ui->playPauseButton, SIGNAL(clicked()), this, SLOT(togglePlayPauseButtonIcon()), Qt::QueuedConnection);
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

        default:
            qDebug("Unhandled tray icon activation: %i", (int)activationReason);
            break;
    }
}

void MainWindow::handlePlayRequest(QModelIndex index) {
    const SoundListModel* currentModel = (SoundListModel*)ui->songView->model();
    const SoundListItem&  soundItem = currentModel->getSongItem(index);

    soundManager->playSound(soundItem.getId());
}

void MainWindow::handleNewSongDuration(int d) {
    ui->progressBar->setRange(0, d);
}

void MainWindow::togglePlayPauseButtonIcon() {
    if (soundManager->isPlaying()) {
        setPauseButtonIcon();
    } else {
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

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleTrayIconActivation(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::setupSoundManager() {
    soundManager = new SoundManager(this);

    connect(soundManager, SIGNAL(started()),  this, SLOT(setPauseButtonIcon()));
    connect(soundManager, SIGNAL(finished()), this, SLOT(setPlayButtonIcon()));
    connect(soundManager, SIGNAL(newSongDuration(int)), this, SLOT(handleNewSongDuration(int)));
    connect(soundManager, SIGNAL(playTimeElapsed(int)), ui->progressBar, SLOT(setValue(int)));

    connect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(play()));
    connect(ui->nextButton,      SIGNAL(clicked()), soundManager, SLOT(next()));
    connect(ui->prevButton,      SIGNAL(clicked()), soundManager, SLOT(previous()));
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
