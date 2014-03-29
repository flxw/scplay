# include "mainwindow.h"
# include "ui_mainwindow.h"

# include <QMenu>
# include <QPropertyAnimation>
# include <QParallelAnimationGroup>
# include <QSettings>
# include <QStandardPaths>

# include "soundcloudapi.h"
# include "enterusernamewidget.h"
# include "playerwidget.h"
# include "introwidget.h"
# include "soundlistdelegate.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent, Qt::FramelessWindowHint), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->hide();

    // --- other setups
    setupTrayIcon();
    setupSoundListView();
    setupSoundListViewControls();

#ifdef QT_DEBUG
    SoundCloudApi::getInstance().setUserId(62853215);
#else
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/scplay/config.ini", QSettings::IniFormat);

    int uid = settings.value("userId", -1).toInt();

    if (uid < 0) {
        setupWelcomeScreen();
    } else {
        SoundCloudApi::getInstance().setUserId(uid);
    }
#endif
}

MainWindow::~MainWindow() {
#ifndef QT_DEBUG
    int uid = SoundCloudApi::getInstance().getUserId();

    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/scplay/config.ini", QSettings::IniFormat);
    settings.setValue("userId", uid);
#endif

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

void MainWindow::handleAnimationEnd() {
    ui->footerLabel->setPixmap(QPixmap(":/icons/grey_130x20.png"));
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

void MainWindow::setupSoundListView() {
    soundModel = new SoundModel(this);

    ui->songView->setModel(soundModel);
    ui->songView->setItemDelegate(new SoundListDelegate(this));

    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);
    QPropertyAnimation *slideInAnimation = new QPropertyAnimation(ui->playerWidget, "geometry", animationGroup);
    slideInAnimation->setDuration(500);
    slideInAnimation->setStartValue(QRect(QPoint(0, -140), ui->playerWidget->size()));
    slideInAnimation->setEndValue(QRect(QPoint(0,0), ui->playerWidget->size()));

    QPropertyAnimation *shrinkAnimation = new QPropertyAnimation(ui->soundViewAndControls, "geometry", animationGroup);
    shrinkAnimation->setDuration(500);
    shrinkAnimation->setStartValue(ui->soundViewAndControls->geometry());
    shrinkAnimation->setEndValue(QRect(QPoint(10,140), ui->soundViewAndControls->size() - QSize(0,130)));

    animationGroup->addAnimation(slideInAnimation);
    animationGroup->addAnimation(shrinkAnimation);

    connect(ui->songView, SIGNAL(doubleClicked(QModelIndex)), ui->playerWidget, SLOT(handlePlayRequest(QModelIndex)));
    connect(ui->songView, SIGNAL(doubleClicked(QModelIndex)), animationGroup, SLOT(start()));
    connect(animationGroup, SIGNAL(finished()), animationGroup, SLOT(deleteLater()));
    connect(animationGroup, SIGNAL(finished()), this, SLOT(handleAnimationEnd()));
    connect(&SoundCloudApi::getInstance(), SIGNAL(isReady()), soundModel, SLOT(fill()));
}

void MainWindow::setupWelcomeScreen() {
    QFrame*  helloUserFrame  = new QFrame(this);
    QWidget* helloUserScreen = new EnterUserNameWidget(helloUserFrame);
    IntroWidget* introScreen = new IntroWidget(helloUserFrame);

    introScreen->setGeometry(300,0,300,400);

    QPropertyAnimation *slideOutAnimation1 = new QPropertyAnimation(helloUserFrame, "geometry");
    QPropertyAnimation *slideOutAnimation2 = new QPropertyAnimation(helloUserFrame, "geometry");

    slideOutAnimation1->setDuration(500);
    slideOutAnimation1->setStartValue(QRect(0,0, 600, 400));
    slideOutAnimation1->setEndValue(QRect(-300, 0, 600, 400));

    slideOutAnimation2->setDuration(500);
    slideOutAnimation2->setStartValue(QRect(-300,0, 600, 400));
    slideOutAnimation2->setEndValue(QRect(-600, 0, 600, 400));

    helloUserFrame->setMinimumSize(QSize(600,400));
    helloUserFrame->setStyleSheet("background-color: #333;");

    connect(&SoundCloudApi::getInstance(), SIGNAL(isReady()), slideOutAnimation1, SLOT(start()));
    connect(slideOutAnimation1, SIGNAL(finished()), slideOutAnimation1, SLOT(deleteLater()));
    connect(slideOutAnimation1, SIGNAL(finished()), introScreen, SLOT(setFocus()));
    connect(introScreen, SIGNAL(introDone()), slideOutAnimation2, SLOT(start()));
    connect(slideOutAnimation2, SIGNAL(finished()), helloUserFrame, SLOT(deleteLater()));
}

void MainWindow::setupSoundListViewControls() {
    connect(ui->likeButton, SIGNAL(toggled(bool)), this, SLOT(catchFalseLikeListSelectionToggles(bool)));
    connect(ui->playlistButton, SIGNAL(toggled(bool)), this, SLOT(catchFalsePlayListSelectionToggles(bool)));
}

void MainWindow::handleTrayIconSingleClick() {
    QPoint cursorPosition = QCursor::pos();

    // move() moves the left-upper corner of the window to the specified position
    this->move(cursorPosition.x() - width(), cursorPosition.y() + 20);
    this->show();
}

// --- private slots
void MainWindow::catchFalseLikeListSelectionToggles(bool toggle) {
    if (toggle) {
        soundModel->switchToLikeFeed();
    }
}

void MainWindow::catchFalsePlayListSelectionToggles(bool toggle) {
    if (toggle) {
        soundModel->switchToPlaylistFeed();
    }
}
