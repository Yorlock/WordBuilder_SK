#include "waitingroom.h"
#include "ui_waitingroom.h"

waitingroom::waitingroom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waitingroom)
{
    ui->setupUi(this);
}

waitingroom::~waitingroom()
{
    delete ui;
}
