# include "introwidget.h"
# include "ui_introwidget.h"

IntroWidget::IntroWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntroWidget)
{
    ui->setupUi(this);
}

IntroWidget::~IntroWidget()
{
    delete ui;
}

void IntroWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        emit introDone();
    } else {
        QWidget::keyReleaseEvent(event);
    }
}
