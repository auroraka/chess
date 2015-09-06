#ifndef MUSIC_H
#define MUSIC_H

#include <QSound>
#include <map>
using std::map;

enum MusicStatus{Open=0,Close=1};

class Music
{
public:
    Music();
    void changeStatus();//音乐开/关
    void loadMusic();//载入音乐
    void playMusic(QString name);//播放音乐name
    MusicStatus getStatus();//音乐播放状态(播放/停止)
    void addMusic(QString,QString);

private:
    MusicStatus status;
    map<QString,QString> musicList;
    QSound *background;
};


extern Music *music;
#endif // MUSIC_H
