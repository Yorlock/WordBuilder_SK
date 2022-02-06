#include "gamewindow.h"
#include "ui_gamewindow.h"

gamewindow::gamewindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gamewindow)
{
    ui->setupUi(this);
    setUpGUI();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(decreaseTime()));
    //addLettersToUse("TEdST");
    //addBuildedWord("TEasdasdasdasdST");
}

gamewindow::~gamewindow()
{
    delete ui;
}

void gamewindow::setUpGUI()
{
    qfont.setBold(true);
    qfont.setPixelSize(24);
}

void gamewindow::addLettersToUse(std::string letters)
{
    for(char letter : letters)
    {
     QChar qchar(letter);
     QLabel *qlabel = new QLabel(qchar,
                              this);
     ui->lettersToUseLayout->addWidget(qlabel);
     ui->lettersToUseLayout->setAlignment(qlabel, Qt::AlignHCenter | Qt::AlignHCenter);
     qlabel->setFont(qfont);
    }
}

void gamewindow::removeFromLettersToUseLayout()
{
    QLayoutItem* child;
    while ( ui->lettersToUseLayout->count() != 0 ) {
        child = ui->lettersToUseLayout->takeAt ( 0 );
        if ( child->layout() != 0 ) {
            ui->lettersToUseLayout->removeItem ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }
        delete child;
    }
}

void gamewindow::decreaseTime()
{
    int time = ui->timeLabelValue->text().toInt();
    time--;
    if(time > 0) ui->timeLabelValue->setText(QString::number(time));
}

void gamewindow::removeFromBuildedWordsLayout()
{
    QLayoutItem* child;
    while ( ui->buildedWordsLayout->count() != 0 ) {
        child = ui->buildedWordsLayout->takeAt ( 0 );
        if ( child->layout() != 0 ) {
            ui->buildedWordsLayout->removeItem ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }
        delete child;
    }
}

void gamewindow::removeFromSolvedVerticalLayout()
{
    QLayoutItem* child;
    while ( ui->solvedVerticalLayout->count() != 0 ) {
        child = ui->solvedVerticalLayout->takeAt ( 0 );
        if ( child->layout() != 0 ) {
            ui->solvedVerticalLayout->removeItem ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }
        delete child;
    }
}

void gamewindow::removeFromRankingVerticalLayout()
{
    QLayoutItem* child;
    while ( ui->rankingVerticalLayout->count() != 0 ) {
        child = ui->rankingVerticalLayout->takeAt ( 0 );
        if ( child->layout() != 0 ) {
            ui->rankingVerticalLayout->removeItem ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }
        delete child;
    }
}

void gamewindow::changeRound(QString round)
{
    ui->roundLabelValue->setText(round);
}

void gamewindow::changeRoundTime(QString time)
{
    ui->timeLabelValue->setText(time);
    timer->start(1000);
}

void gamewindow::addPlayerToRanking(QString word)
{
    QLabel *qlabel = new QLabel(word,
                             this);
    ui->rankingVerticalLayout->addWidget(qlabel);
    ui->rankingVerticalLayout->setAlignment(qlabel, Qt::AlignHCenter | Qt::AlignTop);
    qlabel->setFont(qfont);
}

void gamewindow::addWordToPreviousRound(QString word)
{
    QLabel *qlabel = new QLabel(word,
                             this);
    ui->solvedVerticalLayout->addWidget(qlabel);
    ui->solvedVerticalLayout->setAlignment(qlabel, Qt::AlignHCenter | Qt::AlignTop);
    qlabel->setFont(qfont);
}

void gamewindow::addGuessedWord(QString word)
{
    QLabel *qlabel = new QLabel(word,
                             this);
    ui->buildedWordsLayout->addWidget(qlabel);
    ui->buildedWordsLayout->setAlignment(qlabel, Qt::AlignHCenter | Qt::AlignTop);
    qlabel->setFont(qfont);
}

void gamewindow::addBuildedWord(QString word)
{
    QLabel *qlabel = new QLabel(word,
                             this);
    ui->buildedWordsLayout->addWidget(qlabel);
    ui->buildedWordsLayout->setAlignment(qlabel, Qt::AlignHCenter | Qt::AlignTop);
    qlabel->setFont(qfont);
}

void gamewindow::on_guessPushButton_clicked()
{
    QString word = ui->inputLineEdit->text().trimmed().toUpper();
    ui->inputLineEdit->setText("");
    if(word.compare("") == 1)
    {
        emit sendWordToMainWindow(word);
    }

}
