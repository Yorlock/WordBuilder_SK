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
    ui->portLineEdit->setEnabled(false);
    ui->ipLineEdit->setEnabled(false);

    QString nick = ui->nickLineEdit->text().trimmed();
    QString ip = ui->ipLineEdit->text().trimmed();
    QString port = ui->portLineEdit->text().trimmed();
    if(nick.compare("") == 1 && ip.compare("") == 1 && port.compare("") == 1)
    {
        emit connectToServer(nick, ip, port.toInt());
    }
    else
    {
        showMessage("Wypelnij wszystkie pola!");
    }
}

void login::showMessage(QString message)
{
    ui->joinLobbyPushButton->setEnabled(true);
    ui->nickLineEdit->setEnabled(true);
    ui->portLineEdit->setEnabled(true);
    ui->ipLineEdit->setEnabled(true);
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
