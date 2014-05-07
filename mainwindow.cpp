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

# define VERSION "0.1.6"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent, Qt::FramelessWindowHint), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->hide();

    setupAnimation();
    setupTrayIcon();
    setupReloadButton();

    if (!SoundCloudApi::getInstance().isAuthenticated()) {
        setupWelcomeScreen();
    } else {
        ui->soundListView->refresh();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

// --- public functions
bool MainWindow::event(QEvent* e) {
#ifndef QT_DEBUG
    switch(e->type()) {
        case QEvent::WindowActivate :
            // gained focus
            break ;

        case QEvent::WindowDeactivate :
            this->hide();
            break ;
    };
#endif

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

void MainWindow::displayNewSongNotification(QString title, QString user) {
    if (!this->isVisible()) {
        trayIcon->showMessage(QString("Now playing %1").arg(user), title);
    }
}

// --- private functions
void MainWindow::setupTrayIcon() {
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/white.png"), this);

    QMenu* trayMenu = new QMenu();
    QAction* versionBar = new QAction("scplay " VERSION, this);
    versionBar->setDisabled(true);

    trayMenu->addAction("\u25B6 / \u2016",    ui->playerWidget, SLOT(togglePlayPause()));
    trayMenu->addAction("\u25B6\u25B6\u007C", ui->playerWidget, SLOT(playNextSong()));
    trayMenu->addAction("\u007C\u25C0\u25C0", ui->playerWidget, SLOT(playPreviousSong()));
    trayMenu->addAction("Exit", this, SLOT(close()));
    trayMenu->addAction(versionBar);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(ui->playerWidget, SIGNAL(songChanged(QString,QString)),
            this, SLOT(displayNewSongNotification(QString,QString)));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(handleTrayIconActivation(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::setupAnimation() {
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

    connect(animationGroup, SIGNAL(finished()), animationGroup, SLOT(deleteLater()));

    connect(ui->playerWidget, SIGNAL(playbackStarted()), animationGroup, SLOT(start()));
    connect(ui->likeButton,     SIGNAL(clicked()), ui->soundListView, SLOT(switchToLikeDisplay()));
    connect(ui->playlistButton, SIGNAL(clicked()), ui->soundListView, SLOT(switchToPlaylistListingDisplay()));
    connect(ui->streamButton,   SIGNAL(clicked()), ui->soundListView, SLOT(switchToActivityDisplay()));
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

    connect(&SoundCloudApi::getInstance(), SIGNAL(authenticated()), slideOutAnimation1, SLOT(start()));
    connect(&SoundCloudApi::getInstance(), SIGNAL(authenticated()), ui->soundListView, SLOT(refresh()));

    connect(slideOutAnimation1, SIGNAL(finished()), slideOutAnimation1, SLOT(deleteLater()));
    connect(slideOutAnimation1, SIGNAL(finished()), introScreen, SLOT(setFocus()));
    connect(slideOutAnimation2, SIGNAL(finished()), helloUserFrame, SLOT(deleteLater()));

    connect(introScreen, SIGNAL(introDone()), slideOutAnimation2, SLOT(start()));
}

void MainWindow::setupReloadButton() {
    connect(ui->reloadButton, SIGNAL(clicked()), ui->soundListView, SLOT(refresh()));
}

void MainWindow::handleTrayIconSingleClick() {
    static QPoint windowPosition;

    // initialize the static var windowPosition first
    if (windowPosition.isNull()) {
        QPoint iconPosition = trayIcon->geometry().center();
        QSize mySize = this->size();

        if (iconPosition.x() - mySize.width() < 0) {
            // icon is located in the left corner
            if (iconPosition.y() - mySize.height() < 0) {
                // icon is located top-left
                windowPosition = QPoint(iconPosition.x(), iconPosition.y() + 20);
            } else {
                // icon is located somwhere lower-left
                windowPosition = QPoint(iconPosition.x(), iconPosition.y() - 20 - mySize.height());
            }
        } else {
            // icon is located somewhere right of the left corner
            if (iconPosition.y() - mySize.height() < 0) {
                // icon is located top-right
                windowPosition = QPoint(iconPosition.x() - mySize.width(), iconPosition.y() + 20);
            } else {
                // icon is located somwhere lower-right
                windowPosition = QPoint(iconPosition.x() - mySize.width(), iconPosition.y() - 20 - mySize.height());
            }
        }
    }

    this->move(windowPosition);
    this->show();
}
