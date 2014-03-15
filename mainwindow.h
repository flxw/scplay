#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QSystemTrayIcon>

# include "likelistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool event(QEvent* e);

public slots:
    void handleTrayIconActivation(QSystemTrayIcon::ActivationReason activationReason);

    // --- private section ----------------------
private:
    void setupTrayIcon();
    void setupSoundListView();
    void setupWelcomeScreen();
    void handleTrayIconSingleClick(void);

    // --- attributes
private:
    LikeListModel *likeListModel;

    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
