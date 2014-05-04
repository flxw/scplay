# include "enterusernamewidget.h"
# include "ui_enterusernamewidget.h"
# include "soundcloudapi.h"

EnterUserNameWidget::EnterUserNameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnterUserNameWidget)
{
    ui->setupUi(this);

    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(beginAuth()));
}

EnterUserNameWidget::~EnterUserNameWidget()
{
    delete ui;
}

void EnterUserNameWidget::beginAuth() {
    SoundCloudApi::getInstance().requestAuthentification();
}
