#ifndef ENDGAMERANKING_H
#define ENDGAMERANKING_H

#include <QWidget>

namespace Ui {
class endGameRanking;
}

class endGameRanking : public QWidget
{
    Q_OBJECT

public:
    explicit endGameRanking(QWidget *parent = nullptr);
    ~endGameRanking();
    void addPlayerToRanking(QString word);

private:
    Ui::endGameRanking *ui;
    QFont qfont;
    void setUpGUI();
};

#endif // ENDGAMERANKING_H
