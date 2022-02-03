#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include <QWidget>

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

private:
    Ui::waitingroom *ui;
};

#endif // WAITINGROOM_H
