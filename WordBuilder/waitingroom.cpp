#include "waitingroom.h"
#include "ui_waitingroom.h"

waitingroom::waitingroom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waitingroom)
{
    ui->setupUi(this);
    setUpGUI();
}

waitingroom::~waitingroom()
{
    delete ui;
}

void waitingroom::setUpGUI()
{
    ui->errorLabel->setStyleSheet("QLabel { color : blue; }");
    ui->errorLabel->hide();
    qfont.setBold(true);
    qfont.setPixelSize(24);
}

void waitingroom::setUpGameMaster(bool isGameMaster)
{
    this->isGameMaster = isGameMaster;
    if(isGameMaster)
    {
        ui->liczbaRundspinBox->setDisabled(false);
        ui->czasRundyspinBox->setDisabled(false);
        ui->startGamePushButton->setDisabled(false);
    }
    else
    {
        ui->liczbaRundspinBox->setDisabled(true);
        ui->czasRundyspinBox->setDisabled(true);
        ui->startGamePushButton->setDisabled(true);
    }
}

void waitingroom::addPlayerToList(QString word)
{
    numberOfPlayers++;
    QLabel *qlabel = new QLabel(word,
                             this);
    qlabel->setAccessibleName(word);
    ui->listOfPlayers->addWidget(qlabel);
    ui->listOfPlayers->setAlignment(qlabel, Qt::AlignHCenter | Qt::AlignTop);
    qlabel->setFont(qfont);
}

void waitingroom::changeRoundNumber(QString roundNumber)
{
    ui->liczbaRundspinBox->setValue(roundNumber.toInt());
}

void waitingroom::changeRoundTime(QString roundTime)
{
    ui->czasRundyspinBox->setValue(roundTime.toInt());
}

void waitingroom::erasePlayer(QString nick)
{
    std::cout<< "usuwanie gracza" <<"\n";
    QWidget *widget;
    for(int i = 0; i < ui->listOfPlayers->count(); i++)
    {
        widget = ui->listOfPlayers->itemAt(i)->widget();
        if(widget->accessibleName() == nick) break;
    }
    widget->hide();
    ui->listOfPlayers->removeWidget(widget);
}

void waitingroom::on_liczbaRundspinBox_valueChanged(int roundNumber)
{
    if(isGameMaster)
    {
        emit roundNumberChanged(roundNumber);
    }
}

void waitingroom::on_czasRundyspinBox_valueChanged(int roundTime)
{
    if(isGameMaster)
    {
        emit roundTimeChanged(roundTime);
    }
}

void waitingroom::on_startGamePushButton_clicked()
{
    if(numberOfPlayers > 1)
    {
        emit gameStarted();
    }
    else
    {
        ui->errorLabel->setText("Za mało graczy!");
        ui->errorLabel->show();
        QTimer::singleShot(5000, ui->errorLabel, &QLabel::hide);
    }
    std::cout << "Number of players: " << numberOfPlayers <<"\n";
}
