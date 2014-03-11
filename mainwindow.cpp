#include "mainwindow.h"
#include "ui_mainwindow.h"

# include <QJsonDocument>
# include <QJsonObject>
# include <QJsonValue>
# include <QNetworkRequest>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    nwa = new QNetworkAccessManager(this);

    setupTrayIcon();

    // --- connections
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(startPlaying()));
    connect(nwa, SIGNAL(finished(QNetworkReply*)), this, SLOT(processStreamLocationRequestAnswer(QNetworkReply*)));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleTrayIconActivation(QSystemTrayIcon::ActivationReason)));

    nwa->get(QNetworkRequest(QUrl("http://api.sndcdn.com/i1/tracks/137766501/streams?client_id=798808c14d25fc803a4f484e821ca63a")));
    this->hide();
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
    }

    return QMainWindow::event(e) ;
}

// --- public slots
void MainWindow::startPlaying()
{
    player->setMedia(QUrl(streamLocation));
    player->play();
}

void MainWindow::processStreamLocationRequestAnswer(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray replyBytes = reply->readAll();
        QString replyString = QString(replyBytes);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(replyString.toUtf8());

        qDebug(replyString.toStdString().c_str());

        QJsonValue streamUrl = jsonDocument.object()["http_mp3_128_url"];

        if (streamUrl.isString()) {
            streamLocation = streamUrl.toString();
            this->ui->pushButton->setEnabled(true);
        }
    }
}

void MainWindow::handleTrayIconActivation(QSystemTrayIcon::ActivationReason activationReason) {
    switch (activationReason) {
        case QSystemTrayIcon::Trigger:
            handleTrayIconSingleClick();
            break;
        default: qDebug("Unhandled tray icon activation: %i", (int)activationReason);

    }
}

// --- private functions
void MainWindow::setupTrayIcon() {
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/white.png"), this);
    trayIcon->show();
}

void MainWindow::handleTrayIconSingleClick() {
    QPoint cursorPosition = QCursor::pos();

    // move() moves the left-upper corner of the window to the specified position
    this->move(cursorPosition.x() - width(), cursorPosition.y() + 20);
    this->show();
}
