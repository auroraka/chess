#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include "record.h"
#include "gameinfo.h"
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <vector>
using std::vector;

namespace Ui {
class GameInterface;
}

class GameInterface : public QWidget
{
    Q_OBJECT

public:
    explicit GameInterface(QWidget *parent = 0);
    ~GameInterface();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void myUpdate();
    void randomOne();
    void moveOneStep(int x,int y);
    void regretEvent();
    void readFile(QString fileDir);
    bool readSave();
    void saveRecord();
    void gameRestart();
    void prepareAdd(QString name);
    void talk(QString);
    void saveNow(QString);
    void openBackup(QString fileDir);
    void keyPressEvent(QKeyEvent *event);
    void regretButtonOn();


signals:
    void client_package_gameInfo_change();
    void client_package_update_gameInfo();

public slots:
    void gameStart();
    void gameEnd();
    bool checkGameOver();


private slots:

    void on_send_button_clicked();

    void on_gamePrepare_button_clicked();

    void on_regretRequesst_button_clicked();

    void on_exitRequest_button_clicked();

    void on_saveGame_clicked();

    void on_loadGame_button_clicked();

public:
    QPoint posToLoc(QPoint pos);
    QPoint locToPos(QPoint loc);
    QPoint locToPos(int x,int y){return locToPos(QPoint(x,y));}
    Ui::GameInterface *ui;
    QLabel *pic[GameSize][GameSize][2];
    QLabel *star[GameSize][GameSize];
    QPoint moveLoc;
    QPoint recStar;
    int nowColor;
    int winColor;
    int myColor;
    QTimer timer;
    QDateTime startTime;
    int timeLeft;
    int isStart;
    int gameRec[250][15][15];
    int step;
    int totRegret;
    int totRecord;
    vector<Record> oldRecord;
    int totPrepare;
    QString userName;
    int timeUse;
    int isLoad;
    QTimer heartBeat;
};

extern GameInterface *gameI;
#endif // GAMEINTERFACE_H
