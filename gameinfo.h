#ifndef GAMEINFO_H
#define GAMEINFO_H

#include "myobject.h"
#include "enviroment.h"
#include <QTextEdit>
#include <QTextStream>

class GameInfo:public MyObject
{
    Q_OBJECT
public:
    GameInfo();    
    QString toString();
    void receiveString(QTextStream &stream);

    QTextEdit *chatWindow;
    int map[GameSize][GameSize];

    int playingOne[2];
    int gaming;

signals:
    void gameStart();
    void gameEnd();

private:
};

extern GameInfo *gameInfo;

#endif // GAMEINFO_H
