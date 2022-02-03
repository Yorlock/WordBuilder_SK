#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setUpGUI();
}

login::~login()
{
    delete ui;
}

void login::setUpGUI()
{
    ui->labelWrongNick->setStyleSheet("QLabel { color : blue; }");
    ui->labelWrongNick->hide();
}

void login::on_joinLobbyPushButton_clicked()
{
    ui->joinLobbyPushButton->setEnabled(false);
    ui->nickLineEdit->setEnabled(false);

    QString nick = ui->nickLineEdit->text().trimmed();
    if(nick.compare("") == 1)
    {
        emit connectToServer(nick);
    }
    else
    {
        showMessage("Nick jest pusty!");
    }
}

void login::showMessage(QString message)
{
    ui->joinLobbyPushButton->setEnabled(true);
    ui->nickLineEdit->setEnabled(true);
    ui->labelWrongNick->setText(message);
    ui->labelWrongNick->show();
    ui->nickLineEdit->setText("");
    QTimer::singleShot(5000, ui->labelWrongNick, &QLabel::hide);

}

void login::nickIsBusy()
{
    isNickBusy = true;
    showMessage("Nick jest zajety!");
}
