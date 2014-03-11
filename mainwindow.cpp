#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->hide();

    // --- other setups
    //setupControlButtons();
    setupTrayIcon();
    setupSoundManager();

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
            this->hide();
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
        default: qDebug("Unhandled tray icon activation: %i", (int)activationReason);

    }
}

void MainWindow::togglePlayPauseButtonIcon() {
    static bool isPlaying = true;

    if (isPlaying) {
        ui->playPauseButton->setText("| |");

        disconnect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(play()));
        connect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(pause()));

        isPlaying = false;
    } else {
        ui->playPauseButton->setText("|>");

        disconnect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(pause()));
        connect(ui->playPauseButton, SIGNAL(clicked()), soundManager, SLOT(play()));

        isPlaying = true;
    }
}

// --- private functions
void MainWindow::setupTrayIcon() {
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/white.png"), this);
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleTrayIconActivation(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::setupSoundManager() {
    soundManager = new SoundManager(this);

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
