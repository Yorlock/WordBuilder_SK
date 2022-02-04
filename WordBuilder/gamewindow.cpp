#include "gamewindow.h"
#include "ui_gamewindow.h"

gamewindow::gamewindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gamewindow)
{
    ui->setupUi(this);
    setUpGUI();
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

void gamewindow::removeFromLayout(QLayout *layout)
{
    QLayoutItem* child;
    while ( layout->count() != 0 ) {
        child = layout->takeAt ( 0 );
        if ( child->layout() != 0 ) {
            layout->removeItem ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }
        delete child;
    }
}

void gamewindow::addBuildedWord(char word[])
{
    QLabel *qlabel = new QLabel(word,
                             this);
    ui->buildedWordsLayout->addWidget(qlabel);
    ui->buildedWordsLayout->setAlignment(qlabel, Qt::AlignHCenter | Qt::AlignTop);
    qlabel->setFont(qfont);
}
