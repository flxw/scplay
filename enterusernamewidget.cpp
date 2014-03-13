# include "enterusernamewidget.h"
# include "ui_enterusernamewidget.h"
# include "soundcloudapi.h"

EnterUserNameWidget::EnterUserNameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnterUserNameWidget)
{
    ui->setupUi(this);

    wobbleAnimation = new QPropertyAnimation(ui->lineEdit, "geometry", this);

    wobbleAnimation->setDuration(750);
    wobbleAnimation->setStartValue(QRect(32, ui->lineEdit->pos().y(), ui->lineEdit->width(), ui->lineEdit->height()));
    wobbleAnimation->setEndValue(ui->lineEdit->geometry());
    wobbleAnimation->setEasingCurve(QEasingCurve::OutElastic);

    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(checkInput()));
    connect(&SoundCloudApi::getInstance(), SIGNAL(badUserIdGiven()), wobbleAnimation, SLOT(start()));
}

EnterUserNameWidget::~EnterUserNameWidget()
{
    delete ui;
}

void EnterUserNameWidget::checkInput() {
    QString input = ui->lineEdit->text();

    if (!input.contains("/")) {
        SoundCloudApi::getInstance().setUserPermaLink(input);
    } else {
        wobbleAnimation->start();
    }
}
