# include "mainwindow.h"
# include "ui_mainwindow.h"

# include <QMenu>
# include <QPropertyAnimation>

# include "likelistmodel.h"
# include "soundcloudapi.h"
# include "enterusernamewidget.h"
# include "playerwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent, Qt::FramelessWindowHint), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->hide();

    // --- other setups
    setupTrayIcon();
    setupSoundListViews();

#ifndef QT_DEBUG
    setupWelcomeScreen();
#endif

    // -- connections
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

// --- private functions
void MainWindow::setupTrayIcon() {
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/white.png"), this);

    QMenu* trayMenu = new QMenu();

    trayMenu->addAction("\u25B6 / \u2016",    ui->playerWidget, SLOT(togglePlayPause()));
    trayMenu->addAction("\u25B6\u25B6\u007C", ui->playerWidget, SLOT(playNextSong()));
    trayMenu->addAction("\u007C\u25C0\u25C0", ui->playerWidget, SLOT(playPreviousSong()));
    trayMenu->addAction("Exit", this, SLOT(close()));

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(handleTrayIconActivation(QSystemTrayIcon::ActivationReason)));
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
    connect(ui->songView, SIGNAL(doubleClicked(QModelIndex)), ui->playerWidget, SLOT(handlePlayRequest(QModelIndex)));
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
