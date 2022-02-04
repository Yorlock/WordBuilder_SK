#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpGUI()
{
    ui->gamewindowWidget->setVisible(false);
    ui->waitingroomWidget->setVisible(false);
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
    /*
    if(!ui->loginWidget->isNickBusy)
    {
        ui->loginWidget->setVisible(true);
        ui->waitingroomWidget->setVisible(false);
        ui->gamewindowWidget->setVisible(false);
        ui->loginWidget->showMessage("Disconnected");
    }
    ui->loginWidget->isNickBusy = false;
    */

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
            startGameWindowInfo(s.mid(j+1));
        }
    }
}

void MainWindow::loginInfo(QString message)
{
    std::cout << "Login info: " << message.toStdString() <<"\n";
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
    std::cout << "Nick: " << message.toStdString() <<"\n";
    ui->waitingroomWidget->addPlayerToList(message);
}

void MainWindow::roundInfo(QString message)
{
    std::cout << "Round: " << message.toStdString() <<"\n";
    ui->waitingroomWidget->changeRoundNumber(message);
}

void MainWindow::timeInfo(QString message)
{
    std::cout << "Time: " << message.toStdString() <<"\n";
    ui->waitingroomWidget->changeRoundTime(message);
}

void MainWindow::startGameWindowInfo(QString message)
{
    ui->waitingroomWidget->setVisible(false);
    ui->gamewindowWidget->setVisible(true);
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
    //connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::socketError);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadable);
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
