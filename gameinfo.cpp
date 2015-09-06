#include "gameinfo.h"

#include <QTextStream>
#include <QDebug>

GameInfo::GameInfo()
{
    chatWindow = new QTextEdit;
    for (int i=0;i<GameSize;i++)
        for (int j=0;j<GameSize;j++)
            map[i][j]=2;

    playingOne[0]=-1;
    playingOne[1]=-1;
    gaming=0;

}

//QTextEdit *chatWindow;
//int map[GameSize][GameSize];

//int totPrepare;
//int playingOne[2];


QString GameInfo::toString(){
    QString info;
    QTextStream stream(&info);
    stream<<"\n __GameInfo__\n";
    stream<<"\n chatWindow\n";
        QString a=chatWindow->toPlainText();
        a=a.replace(" ","@@");
        a=a.replace("\n","##");
        stream<<a;
        stream<<"\n __End__\n";

    stream<<"\n map\n";
    for (int i=0;i<GameSize;i++)
        for (int j=0;j<GameSize;j++){
            stream<<"\n "+QString::number(map[i][j])+"\n";
        }
    stream<<"\n playingOne\n"<<playingOne[0]<<"\n"<<playingOne[1];
    stream<<"\n __End__\n";
    return info;
}
void GameInfo::receiveString(QTextStream &stream){
    while (stream.status() == QTextStream::Ok){
        QString name;
        stream>>name;
        if (name=="__End__") break;
        if (name=="chatWindow"){
            QString chatInfo;
            while (stream.status() == QTextStream::Ok){
                QString op;
                stream>>op;
                if (op=="__End__") break;
                chatInfo+=op;
            }
            chatInfo=chatInfo.replace("@@"," ");
            chatInfo=chatInfo.replace("##","\n");
            chatWindow->clear();
            chatWindow->append(chatInfo);
        }
        if (name=="map"){
            for (int i=0;i<GameSize;i++)
                for (int j=0;j<GameSize;j++){
                    stream>>map[i][j];
                //    qDebug()<<"map "<<i<<" "<<j<<": "<<map[i][j];
                }
        }
        if (name=="playingOne"){
            stream>>playingOne[0]>>playingOne[1];
           // qDebug()<<"palyingOne: "<<playingOne[0]<<" "<<playingOne[1];
        }
    }

}


