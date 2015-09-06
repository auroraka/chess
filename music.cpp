#include "music.h"


#include <QTextStream>
#include <QDebug>
#include <QFileInfo>

Music::Music()
{
   // qDebug()<<"making music...";
    status=Open;
    loadMusic();
    QString fileDir="../chess/music/bg.wav";
    QFileInfo file(fileDir);
    if (file.exists()){
        background = new QSound(fileDir);
    }else{
        background = new QSound(":/music/music/background.wav");
    }
    background->setLoops(QSound::Infinite);


    background->play();

    qDebug()<<"music maked";
}

void Music::changeStatus(){
    if (status == Open){
        status = Close;
        background->stop();

    }else{
        status = Open;
        background->play();
    }
}

MusicStatus Music::getStatus(){
    return status;
}

QString loadMusicList[]={
    "winsound",
    "failsound",
    "menuback",
    "menuclick",
};
int musicTot=4;


void Music::loadMusic(){
    for (int i=0;i<musicTot;i++){
        QString fileDir;
        QTextStream(&fileDir)<<"../chess/music/"<<loadMusicList[i]<<".wav";
        qDebug()<<"openMusic: "<<fileDir;
        musicList[loadMusicList[i]]=fileDir;
    }
}

void Music::addMusic(QString dir,QString name){
    musicList[name]=dir;
}

void Music::playMusic(QString name){
    if (status == Close) return;
    if (musicList.find(name) == musicList.end()){
        qDebug()<<"[ERROR] music not found: "<<name;
        return;
    }
    qDebug()<<"play music: "<<name;
    QSound::play(musicList[name]);
}
