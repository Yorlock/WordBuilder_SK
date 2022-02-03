#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"

#include <QTcpSocket>
#include <iostream>
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
    std::cout << "Send nick: " << playerNick.toStdString()<<"\n";
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
    buffor.append(wholeMessage);
    cout << "Whole message: " << wholeMessage.toStdString() << "\n";
    QStringList list;
    list = buffor.split('\n');
    buffor = list.last();
    std::cout <<"buffor: " << buffor.toStdString() <<"\n";
    for(int i=0; i<list.size()-2; i++)
    {
        QString s = list[i];
        if(s[0] == "l")
        {
            loginInfo(s.mid(1));
        }
        else if(s[0] == "n")
        {
            waintingRoomInfo();
        }
        else if(s[0] == "g")
        {
            gameWindowInfo();
        }
    }
}

void MainWindow::splitMessage()
{

}

void MainWindow::loginInfo(QString message)
{
    std::cout << "Message: " << message.toStdString() <<"\n";
    if(message == "1")
    {
        ui->waitingroomWidget->isGameMaster = true;
        ui->loginWidget->setVisible(false);
        ui->waitingroomWidget->setVisible(true);
    }
    else if(message == "2")
    {
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

void MainWindow::waintingRoomInfo()
{
    QByteArray message;
    message = socket->read(29);
    std::cout << "Nick: " << message.toStdString() <<"\n";

}

void MainWindow::gameWindowInfo()
{
    QByteArray message;
    message = socket->read(1);
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
