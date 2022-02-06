#include "endgameranking.h"
#include "ui_endgameranking.h"

endGameRanking::endGameRanking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::endGameRanking)
{
    ui->setupUi(this);
    setUpGUI();
}

endGameRanking::~endGameRanking()
{
    delete ui;
}

void endGameRanking::addPlayerToRanking(QString word)
{
    QLabel *qlabel = new QLabel(word,
                             this);
    ui->rankingLayout->addWidget(qlabel);
    ui->rankingLayout->setAlignment(qlabel, Qt::AlignHCenter | Qt::AlignTop);
    qlabel->setFont(qfont);
}

void endGameRanking::setUpGUI()
{
    qfont.setBold(true);
    qfont.setPixelSize(28);
}
