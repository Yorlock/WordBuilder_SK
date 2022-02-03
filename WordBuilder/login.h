#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void showMessage(QString message);
    void nickIsBusy();
    bool isNickBusy = false;

private slots:
    void on_joinLobbyPushButton_clicked();

signals:
    void connectToServer(QString nick);

private:
    Ui::login *ui;
    void setUpGUI();
};

#endif // LOGIN_H
