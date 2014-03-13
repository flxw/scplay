#ifndef ENTERUSERNAMEWIDGET_H
#define ENTERUSERNAMEWIDGET_H

# include <QWidget>
# include <QPropertyAnimation>


namespace Ui {
class EnterUserNameWidget;
}

class EnterUserNameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EnterUserNameWidget(QWidget *parent = 0);
    ~EnterUserNameWidget();

private slots:
    void checkInput();

private:
    Ui::EnterUserNameWidget *ui;
    QPropertyAnimation* wobbleAnimation;
};

#endif // ENTERUSERNAMEWIDGET_H
