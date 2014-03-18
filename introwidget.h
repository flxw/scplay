#ifndef INTROWIDGET_H
#define INTROWIDGET_H

# include <QWidget>
# include <QKeyEvent>

namespace Ui {
class IntroWidget;
}

class IntroWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IntroWidget(QWidget *parent = 0);
    ~IntroWidget();

    void keyReleaseEvent(QKeyEvent *event);

signals:
    void introDone();

private:
    Ui::IntroWidget *ui;
};

#endif // INTROWIDGET_H
