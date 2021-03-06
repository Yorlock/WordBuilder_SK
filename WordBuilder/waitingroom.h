#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include <QWidget>
#include <iostream>
#include <QTimer>

namespace Ui {
class waitingroom;
}

class waitingroom : public QWidget
{
    Q_OBJECT

public:
    explicit waitingroom(QWidget *parent = nullptr);
    ~waitingroom();

    bool isGameMaster = false;
    void addPlayerToList(QString word);
    void setUpGameMaster(bool isGameMaster);
    void changeRoundNumber(QString roundNumber);
    void changeRoundTime(QString roundTime);
    void erasePlayer(QString nick);
    int numberOfPlayers = 0;

private slots:
    void on_liczbaRundspinBox_valueChanged(int arg1);

    void on_czasRundyspinBox_valueChanged(int arg1);

    void on_startGamePushButton_clicked();

signals:
    void roundNumberChanged(int);
    void roundTimeChanged(int);
    void gameStarted();

private:
    QFont qfont;

    Ui::waitingroom *ui;

    void setUpGUI();
};

#endif // WAITINGROOM_H
