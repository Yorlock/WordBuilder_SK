#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <string>
#include <QTimer>
#include <string>

namespace Ui {
class gamewindow;
}

class gamewindow : public QWidget
{
    Q_OBJECT

public:
    explicit gamewindow(QWidget *parent = nullptr);
    ~gamewindow();
    void addLettersToUse(std::string letters);
    void removeFromLettersToUseLayout();
    void removeFromBuildedWordsLayout();
    void removeFromSolvedVerticalLayout();
    void removeFromRankingVerticalLayout();
    void addPlayerToRanking(QString word);
    void addWordToPreviousRound(QString word);
    void addGuessedWord(QString word);
    void addBuildedWord(QString word);
    void changeRound(QString round);
    void changeRoundTime(QString time);
public slots:
    void decreaseTime();

signals:
    void sendWordToMainWindow(QString word);

private slots:
    void on_guessPushButton_clicked();

private:
    Ui::gamewindow *ui;
    void setUpGUI();
    QFont qfont;
    QFont qfontNick;
    QTimer *timer;

};

#endif // GAMEWINDOW_H
