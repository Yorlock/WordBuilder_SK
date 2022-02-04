#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
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
    void removeFromLayout(QLayout *layout);
    void addBuildedWord(char word[]);

private:
    Ui::gamewindow *ui;
    void setUpGUI();
    QFont qfont;

};

#endif // GAMEWINDOW_H
