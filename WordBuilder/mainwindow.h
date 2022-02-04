#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <iostream>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket * socket {nullptr};
    QTimer * connTimeoutTimer {nullptr};
    QString playerNick;
    QString buffor;

    void splitMessage();
    void setUpGUI();
    void socketConnected();
    void socketDisconnected();
    void socketError(QTcpSocket::SocketError);
    void socketReadable();
    void tryConnectToServer(QString nick);

    void loginInfo(QString message);
    void nickInfo(QString message);
    void roundInfo(QString round);
    void timeInfo(QString time);
    void startGameWindowInfo(QString message);
    void sendRoundNumber(int roundNumber);
    void sendRoundTime(int roundTime);
    void sendGameStarted();

};
#endif // MAINWINDOW_H
