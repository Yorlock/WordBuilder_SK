#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    setUpGUI();
    QObject::connect(ui->loginWidget, &login::connectToServer, this, &MainWindow::tryConnectToServer);
    QObject::connect(ui->waitingroomWidget, &waitingroom::roundNumberChanged, this, &MainWindow::sendRoundNumber);
    QObject::connect(ui->waitingroomWidget, &waitingroom::roundTimeChanged, this, &MainWindow::sendRoundTime);
    QObject::connect(ui->waitingroomWidget, &waitingroom::gameStarted, this, &MainWindow::sendGameStarted);
    QObject::connect(ui->gamewindowWidget, &gamewindow::sendWordToMainWindow, this, &MainWindow::sendWordToServer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpGUI()
{
    ui->gamewindowWidget->setVisible(false);
    ui->waitingroomWidget->setVisible(false);
    ui->endGameRankingWidget->setVisible(false);
}

void MainWindow::socketConnected()
{
    connTimeoutTimer->stop();
    connTimeoutTimer->deleteLater();
    connTimeoutTimer=nullptr;
    socket->write(playerNick.toStdString().c_str());
}

void MainWindow::socketDisconnected()
{
    if(!ui->loginWidget->isNickBusy)
    {
        ui->loginWidget->setVisible(true);
        ui->waitingroomWidget->setVisible(false);
        ui->gamewindowWidget->setVisible(false);
        ui->loginWidget->showMessage("Disconnected");
    }
    ui->loginWidget->isNickBusy = false;
}

void MainWindow::socketReadable()
{
    QByteArray wholeMessage;
    wholeMessage = socket->readAll();
    QString helper = QString::fromStdString(wholeMessage.toStdString());
    buffor = buffor + helper;

    QStringList list;
    list = buffor.split('@');
    buffor = list.last();

    for(int i=0; i<list.size()-1; i++)
    {
        QString s = list[i];
        int j = 0;
        while(s[j] == NULL && j < s.size()) j++;
        if(s[j] == "l")
        {
            loginInfo(s.mid(j+1));
        }
        else if(s[j] == "n")
        {
            nickInfo(s.mid(j+1));
        }
        else if(s[j] == "r")
        {
            roundInfo(s.mid(j+1));
        }
        else if(s[j] == "t")
        {
            timeInfo(s.mid(j+1));
        }
        else if(s[j] == "g")
        {
            startGameWindowInfo();
        }
        else if(s[j] == "z")
        {
            currentRoundInGame(s.mid(j+1));
        }
        else if(s[j] == "x")
        {
            currentTimeInGame(s.mid(j+1));
        }
        else if(s[j] == "q")
        {
            currentSetOfLetters(s.mid(j+1));
        }
        else if(s[j] == "c")
        {
            addPlayerToRanking(s.mid(j+1));
        }
        else if(s[j] == "p")
        {
            wordsFromPreviousRound(s.mid(j+1));
        }
        else if(s[j] == "Q")
        {
            clientDisconnect(s.mid(j+1));
        }
        else if(s[j] == "o")
        {
            guessedWord(s.mid(j+1));
        }
        else if(s[j] == "a")
        {
            newGameMaster();
        }
        else if(s[j] == "E")
        {
            gameIsOver();
        }
        else if(s[j] == "C")
        {
            addPlayerToRankingAfterGameFinished(s.mid(j+1));
        }
        else if(s[j] == "L")
        {
            gameIsOverTooLittlePlayers();
        }
    }
}

void MainWindow::loginInfo(QString message)
{
    if(message == "1")
    {
        ui->waitingroomWidget->waitingroom::setUpGameMaster(true);
        ui->loginWidget->setVisible(false);
        ui->waitingroomWidget->setVisible(true);
    }
    else if(message == "2")
    {
        ui->waitingroomWidget->waitingroom::setUpGameMaster(false);
        ui->loginWidget->setVisible(false);
        ui->waitingroomWidget->setVisible(true);
    }
    else if(message == "3")
    {
        ui->loginWidget->setVisible(false);
        ui->gamewindowWidget->setVisible(true);
    }
    else if(message == "4")
    {
       ui->loginWidget->nickIsBusy();
    }
}

void MainWindow::nickInfo(QString message)
{
    ui->waitingroomWidget->addPlayerToList(message);
}

void MainWindow::clientDisconnect(QString message)
{
    ui->waitingroomWidget->erasePlayer(message);
}

void MainWindow::gameIsOver()
{
    disconnect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    ui->gamewindowWidget->setVisible(false);
    ui->loginWidget->setVisible(false);
    ui->waitingroomWidget->setVisible(false);
    ui->endGameRankingWidget->setVisible(true);
}

void MainWindow::gameIsOverTooLittlePlayers()
{
    disconnect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    ui->gamewindowWidget->setVisible(false);
    ui->loginWidget->setVisible(false);
    ui->waitingroomWidget->setVisible(false);
    ui->endGameRankingWidget->setVisible(false);
    QMessageBox::critical(ui->gamewindowWidget, "ERROR", "Za malo graczy! Gra zostala zakonczona.");
}

void MainWindow::roundInfo(QString message)
{
    ui->waitingroomWidget->changeRoundNumber(message);
}

void MainWindow::timeInfo(QString message)
{
    ui->waitingroomWidget->changeRoundTime(message);
}

void MainWindow::startGameWindowInfo()
{
    ui->waitingroomWidget->setVisible(false);
    ui->gamewindowWidget->setVisible(true);
}

void MainWindow::currentRoundInGame(QString message)
{
    ui->gamewindowWidget->changeRound(message);
}

void MainWindow::currentTimeInGame(QString message)
{
    ui->gamewindowWidget->changeRoundTime(message);
}

void MainWindow::currentSetOfLetters(QString message)
{
    ui->gamewindowWidget->removeFromBuildedWordsLayout();
    ui->gamewindowWidget->removeFromLettersToUseLayout();
    ui->gamewindowWidget->removeFromRankingVerticalLayout();
    ui->gamewindowWidget->removeFromSolvedVerticalLayout();
    ui->gamewindowWidget->addLettersToUse(message.toStdString());
}

void MainWindow::addPlayerToRanking(QString message)
{
    ui->gamewindowWidget->addPlayerToRanking(message);
}

void MainWindow::wordsFromPreviousRound(QString message)
{
    ui->gamewindowWidget->addWordToPreviousRound(message);
}

void MainWindow::guessedWord(QString message)
{
    ui->gamewindowWidget->addGuessedWord(message);
}

void MainWindow::newGameMaster()
{
    ui->waitingroomWidget->waitingroom::setUpGameMaster(true);
}

void MainWindow::tryConnectToServer(QString nick)
{
    playerNick = nick;
    if(socket) delete socket;
    socket = new QTcpSocket(this);
    connTimeoutTimer = new QTimer(this);
    connTimeoutTimer->setSingleShot(true);

    connect(connTimeoutTimer, &QTimer::timeout, [&]{
            socket->abort();
            socket->deleteLater();
            socket = nullptr;
            connTimeoutTimer->deleteLater();
            connTimeoutTimer=nullptr;
            ui->loginWidget->setVisible(false);
            ui->loginWidget->showMessage("Connect timed out!");
            QMessageBox::critical(this, "Error", "Connect timed out");
        });
    connect(socket, &QTcpSocket::connected, this, &MainWindow::socketConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadable);
    connTimeoutTimer->start(3000);
    socket->connectToHost("127.0.0.1", 1112);

}

void MainWindow::sendRoundNumber(int roundNumber)
{
    char message[4] = "r";
    strcat_s(message, to_string(roundNumber).c_str());
    socket->write(message);
}

void MainWindow::sendRoundTime(int roundTime)
{
    char message[5] = "t";
    strcat_s(message, to_string(roundTime).c_str());
    socket->write(message);
}

void MainWindow::sendGameStarted()
{
    char message[2] = "g";
    socket->write(message);
}

void MainWindow::sendWordToServer(QString word)
{
    char message[31] = "";
    strcat_s(message, word.toStdString().c_str());
    socket->write(message);
}

void MainWindow::addPlayerToRankingAfterGameFinished(QString message)
{
    ui->endGameRankingWidget->addPlayerToRanking(message);
}
