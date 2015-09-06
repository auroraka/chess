#include "gameinterface.h"
#include "ui_gameinterface.h"
#include "enviroment.h"
#include "gameinfo.h"
#include "tcp.h"
#include "music.h"
#include <QTime>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <vector>
#include <QMessageBox>
#include <QTime>
#include <QFileInfo>
#include <QPushButton>
#include <QFileDialog>
using std::vector;

QString nameList[]={
    "Tom",
    "HuangDaDa",
    "SillB",
    "OMG",
    "keke",
    "yo"
};
int nameTot=6;

GameInterface::GameInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameInterface)
{
    ui->setupUi(this);
    this->setMouseTracking(1);
    ui->gameArea->setMouseTracking(1);
    //this->setMouseTracking(0);
    ui->announcement->setText(tr("公告:游戏尚未开始"));


    //添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("../chess/picture/bg5.jpg")));
    this->setPalette(palette);
    setAcceptDrops(true);


    userName=nameList[qrand()%nameTot];
    readSave();
    isLoad=0;
    connect(&timer,&QTimer::timeout,[=](){
        timeLeft--;
        ui->lcdNumber->display(timeLeft);
        if (nowColor ==myColor){
            if (timeLeft<=0){
                timer.stop();
                ui->announcement->setText("时间到");
                randomOne();
            }
            timeUse++;
        }
    });
    moveLoc = QPoint(-1,-1);
    recStar=QPoint(-1,-1);
    isStart=0;
    nowColor=0;

    gameInfo = new GameInfo;
    //client->package->gameInfo=gameInfo;
    //client->package->gameInfo->chatWindow=ui->chatWindow_textEdit;
    QObject::connect(gameInfo,SIGNAL(gameStart()),this,SLOT(gameStart()));

    for (int i=0;i<15;i++)
        for (int j=0;j<15;j++){
            QLabel *label;
            QPoint pos=locToPos(i,j);

            label=new QLabel(this);;
            label->setPixmap(QPixmap(":picture/picture/star.png"));
            label->setGeometry(pos.x(),pos.y(),BlockLen,BlockLen);
            label->hide();
            star[i][j]=label;

            label=new QLabel(this);;
            label->setPixmap(QPixmap(":picture/picture/black.png"));
            label->setGeometry(pos.x(),pos.y(),BlockLen,BlockLen);
            label->hide();
            pic[i][j][0]=label;

            label=new QLabel(this);;
            label->setPixmap(QPixmap(":picture/picture/white.png"));
            label->setGeometry(pos.x(),pos.y(),BlockLen,BlockLen);
            label->hide();
            pic[i][j][1]=label;
        }
}


GameInterface::~GameInterface()
{
    delete ui;
}



void GameInterface::randomOne(){
    int x,y;
    do{
        x=qrand()%15,y=qrand()%15;
        qDebug()<<"radom choose "<<x<<" "<<y;
    }while(gameInfo->map[x][y]!=2);
    tcp->sendInfo("move_one_step",x,y);
    moveOneStep(x,y);
}

void GameInterface::readFile(QString fileDir){
    QFile *file = new QFile(fileDir);
    Record rec;
    if (file->open(QFile::ReadOnly | QFile::Text))
    {
        file->deleteLater();
        QByteArray buff = file->readAll();
        QTextStream stream( &buff );

        stream>>rec.start;
        stream>>rec.end;
        stream>>rec.step;
        for (int k=1;k<=step;k++){
            for (int i=0;i<15;i++)
                for (int j=0;j<15;j++){
                    stream>>rec.record[k][i][j];
                }
        }
        oldRecord.push_back(rec);
    }else
    {
        qDebug()<<"no file in dir:"<<fileDir;
    }
}

bool GameInterface::readSave(){
    int now=1;
    while (1){
        QString dir="../chess/record/"+QString::number(now)+".txt";
        if (!QFileInfo::exists(dir)) break;
        readFile(dir);
        now++;
    }
    totRecord=now-1;
    qDebug()<<"oldRecordTot: "<<totRecord;
}




void GameInterface::paintEvent(QPaintEvent *event){
}

void GameInterface::myUpdate(){
   // qDebug()<<"update";
    //return;
    for (int i=0;i<GameSize;i++)
        for (int j=0;j<GameSize;j++){
            if (gameInfo->map[i][j]==2){
                pic[i][j][0]->hide();
                pic[i][j][1]->hide();
            }else{
                pic[i][j][gameInfo->map[i][j]]->show();
                pic[i][j][gameInfo->map[i][j]^1]->hide();
            }
           // pic[i][j][0]->show();
        }
    if (moveLoc!=QPoint(-1,-1)){
      //  qDebug()<<"draw star: "<<moveLoc;
        if (recStar!=QPoint(-1,-1)) star[recStar.x()][recStar.y()]->hide();
        star[moveLoc.x()][moveLoc.y()]->show();
        recStar=moveLoc;
    }
}

int dis(QPoint a,QPoint b){
    return abs(a.x()-b.x())+abs(a.y()-b.y());
}

QPoint GameInterface::locToPos(QPoint loc){
    int AreaX=ui->gameArea->pos().x(),AreaY=ui->gameArea->pos().y();
    return QPoint(AreaX+StartX-BlockLen/2+BlockLen*loc.x(),AreaY+StartY-BlockLen/2+BlockLen*loc.y());
}

QPoint GameInterface::posToLoc(QPoint pos){
    //return QPoint(-1,-1);
    int diss;QPoint loc(-1,-1);
    for (int i=0;i<GameSize;i++)
        for (int j=0;j<GameSize;j++){
            QPoint ps=locToPos(i,j);
            //qDebug()<<"pos: "<<pos<<" loc:"<<QPoint(i,j);
            QRect rect(ps.x(),ps.y(),BlockLen,BlockLen);
            if (rect.contains(pos)) loc=QPoint(i,j);
        }
    return loc;
}
void swap(int &a,int &b){
    int t=a;a=b;b=t;
}


int xo[]={1,1,1,0};
int yo[]={-1,0,1,1};

bool GameInterface::checkGameOver(){
    for (int i=0;i<GameSize;i++)
        for (int j=0;j<GameSize;j++) if (gameInfo->map[i][j]!=2){
            for (int dir=0;dir<4;dir++){
                int flag=1;
                for (int k=1;k<=4;k++){
                    int nx=i+k*xo[dir],ny=j+k*yo[dir];
                    if (!(0<=nx&&nx<15&&0<=ny&&ny<15)){
                        flag=0;
                        break;
                    }
                    if (gameInfo->map[i][j]!=gameInfo->map[nx][ny]){
                        flag=0;
                        break;
                    }
                }
                if (flag){
                    winColor=gameInfo->map[i][j];
                    return true;
                }
            }
        }
    return false;
}
int tot=0;
void GameInterface::mouseMoveEvent(QMouseEvent *event){
    //qDebug()<<"mouse move: "<<event->pos();
    moveLoc=posToLoc(event->pos());
    //qDebug()<<"get move loc: "<<moveLoc;
    myUpdate();
}

void GameInterface::moveOneStep(int x,int y){
    Debug("nowColor: "+QString::number(nowColor)+"  | "+QString::number(x)+" "+QString::number(y)+") "+QString::number(gameInfo->map[x][y])+" -> "+QString::number(nowColor));
    if (gameInfo->map[x][y]!=2) return;
    qDebug()<<"step: "<<step<<"->"<<step+1;
    ui->chatWindow_textEdit->append(userName+" 落子"+QString::number(x)+" "+QString::number(y)+"\n");
    for (int i=0;i<15;i++)
        for (int j=0;j<15;j++)
            gameRec[step][i][j]=gameInfo->map[i][j];
    step++;
    timeLeft=20;
    timer.start(1000);
    ui->lcdNumber->display(20);
    gameInfo->map[x][y]=nowColor;
    ui->announcement->setText("");
    myUpdate();
    if (checkGameOver()){
        qDebug()<<"gameOver: you win";
        gameEnd();
    }
    nowColor^=1;
    if (myColor == nowColor){
        ui->chatWindow_textEdit->append("现在轮到您的回合");
    }else{
        ui->chatWindow_textEdit->append("现在轮到对方的回合");
    }
}

void GameInterface::mousePressEvent(QMouseEvent *event){
    if (!isStart) return;
    qDebug()<<"mouse press: "<<event->pos();
    //if (myuser->userID!=gameInfo->playingOne[0]) return;
    QPoint loc=posToLoc(event->pos());
    //qDebug()<<"get press loc: "<<loc;
    moveLoc=QPoint(-1,-1);
    int x=loc.x(),y=loc.y();
    if (loc != QPoint(-1,-1)){
        if (nowColor == myColor && gameInfo->map[x][x]==2){
            qDebug()<<"!!!!!!!!!!!!!   "<<x<<" "<<y<<" "<<gameInfo->map[x][x];
            tcp->sendInfo("move_one_step",x,y);
            moveOneStep(x,y);
        }else{
            qDebug()<<"鼠标点击:不可下"<<" "<<x<<" "<<y<<" "<<gameInfo->map[x][y]<<"| "<<nowColor<<" "<<myColor;
        }
        //swap(gameInfo->playingOne[0],gameInfo->playingOne[1]);

    //    client->package->update(gameInfo);
    }else{
        qDebug()<<"鼠标点击:游戏区域外";
    }
}


void GameInterface::gameStart(){
    for (int i=0;i<15;i++)
        for (int j=0;j<15;j++)
            gameInfo->map[i][j]=2;
    timeUse=0;
    isStart=1;
    totPrepare=0;
    totRegret=0;
    startTime = QDateTime::currentDateTime();
    timeLeft=20;
    timer.start(1000);
    ui->lcdNumber->display(20);
    ui->gamePrepare_button->hide();

    if (myColor==nowColor){
        ui->chatWindow_textEdit->append("游戏开始 现在是你的回合");
    }else{
        ui->chatWindow_textEdit->append("游戏开始 现在是对方的回合");
    }
//    if (gameInfo->playingOne[0]==-1||gameInfo->playingOne[1]==-1){
//        qDebug()<<"[SYS ERROR] playingOne not init";
//        SystemError(this);
//    }
    /*if (myuser->userID==gameInfo->playingOne[0]){
        int a=gameInfo->playingOne[0],b=gameInfo->playingOne[1];
        gameInfo->chatWindow->append("游戏开始 "+userList->list[a].name+" vs "+userList->list[b].name);
        gameInfo->gaming=1;
        client->package->update(gameInfo);
    }*/
}



void GameInterface::saveRecord(){
//    Record rec;
//    rec.start=startTime;
//    rec.end=QDateTime::currentDateTime();
//    rec.step=step;
//    for (int k=1;k<=step;k++)
//        for (int i=0;i<15;i++)
//            for (int j=0;j<15;j++)
//                rec.record[k][i][j]=gameRec[k][i][k];

    QFile *file = new QFile("../chess/record/"+QString::number(totRecord+1)+".txt");
    totRecord++;
    if (file->open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(file);

        stream<<startTime.toString()<<endl;
        stream<<QDateTime::currentDateTime().toString()<<endl;
        stream<<step<<endl;
        for (int k=1;k<=step;k++){
            for (int i=0;i<15;i++){
                for (int j=0;j<15;j++){
                    stream<<gameRec[k][i][j]<<" ";
                }
                stream<<endl;
            }
            stream<<endl;
        }
        file->close();
    }else{
        qDebug()<<"open file fail";
    }
}

void GameInterface::gameRestart(){
    for (int i=0;i<15;i++)
        for (int j=0;j<15;j++){
            gameInfo->map[i][j]=2;
        }
    gameStart();
}

void GameInterface::gameEnd(){
    ui->gamePrepare_button->setEnabled(true);
    ui->gamePrepare_button->setText("准备");
    int timeUse=startTime.secsTo(QDateTime::currentDateTime());
    ui->announcement->setText("time use: "+QString::number(timeUse));
    if (winColor==myColor){
        music->playMusic("win");
    }else{
        music->playMusic("fail");

    }
    saveRecord();
    QMessageBox::about(this,(winColor==myColor)?"您赢了":"您输了","您的思考时间共计: "+QString::number(timeUse));
    ui->gamePrepare_button->show();
    myColor^=1;
    gameRestart();
   // client->package->update(gameInfo);
}

void GameInterface::keyPressEvent(QKeyEvent *event){
    if (event->text()=="\n"){
        emit ui->send_button->clicked();
    }
}

void GameInterface::on_send_button_clicked()
{
    music->playMusic("menuclick");
    QString info=ui->chatEdit_textEdit->toPlainText();
    info=userName+" "+QDateTime::currentDateTime().toString("hh:mm:ss")+"\n"+info+"\n";
    tcp->sendInfo("talk",info);
    talk(info);
    ui->chatEdit_textEdit->clear();
    //client->package->update(gameInfo);
}

void GameInterface::prepareAdd(QString name){
    totPrepare++;
    ui->chatWindow_textEdit->append("玩家 "+name+" 已准备");
    ui->chatWindow_textEdit->append("tot prepared: "+QString::number(totPrepare));
    if (totPrepare>=2){
        gameStart();
    }
}

void GameInterface::on_gamePrepare_button_clicked()
{
    music->playMusic("menuclick");
    tcp->sendInfo("prepare",userName);
    prepareAdd(userName);
    ui->gamePrepare_button->setEnabled(false);
    ui->gamePrepare_button->setText("等待对方准备");
}

void GameInterface::talk(QString info){
    ui->chatWindow_textEdit->append(info);
}

void GameInterface::regretEvent(){
    ui->regretRequesst_button->setEnabled(true);
    step--;
    for (int i=0;i<15;i++)
        for (int j=0;j<15;j++){
            gameInfo->map[i][j]=gameRec[step][i][j];
        }
    nowColor^=1;
    myUpdate();
}

void GameInterface::regretButtonOn(){
    ui->regretRequesst_button->setEnabled(true);
}

void GameInterface::on_regretRequesst_button_clicked()
{
    music->playMusic("menuclick");
    if (nowColor==myColor || step<=0){
        talk("您现在不能反悔");
        return;
    }
    if (totRegret<=1){
        tcp->sendInfo("regret_request");
        ui->regretRequesst_button->setEnabled(false);
        ui->chatWindow_textEdit->append("您申请了反悔,请等待对方同意...");
    }else{
        talk("您已经反悔超过 2 次了");
    }
}

void GameInterface::on_exitRequest_button_clicked()
{
    music->playMusic("menuclick");
    tcp->sendInfo("exit_request");
    ui->exitRequest_button->setEnabled(false);
    gameI->talk("正在向对方申请,请稍等...");
}

void GameInterface::saveNow(QString fileDir){

    QString dd=fileDir+QDateTime::currentDateTime().toString("yyyy-MM-dd.");
    int now=0;
    while (1){
        QString dir=dd+QString::number(now)+".bak";
        if (!QFileInfo::exists(dir)) break;
        now++;
    }
    QString dir=dd+QString::number(now)+".bak";
    qDebug()<<dir;
    QFile *file = new QFile(dir);
    if (file->open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(file);
            stream<<"chess__backup"<<endl;
            for (int i=0;i<15;i++){
                for (int j=0;j<15;j++){
                    stream<<gameInfo->map[i][j]<<" ";
                }
                stream<<endl;
            }
        file->close();
    }else{
        qDebug()<<"open file fail";
    }
}

void GameInterface::on_saveGame_clicked()
{
    music->playMusic("menuclick");
    QString path=QFileDialog::getExistingDirectory(this,"保存到","/");
    qDebug()<<"select path :"<<path;
    saveNow(path);
}

QString makeMap(){
    QString info;
    QTextStream stream(&info);
    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++){
            stream<<gameInfo->map[i][j]<<" ";
        }
        stream<<endl;
    }
    Debug("######send:"+info);
    return info;
}


void GameInterface::openBackup(QString fileDir){
    QFile *file = new QFile(fileDir);
    if (file->open(QFile::ReadOnly | QFile::Text))
    {
        QByteArray buff = file->readAll();
        QTextStream stream( &buff );

        QString oper;
        stream>>oper;
        if (oper!="chess__backup"){
            QMessageBox::information(0,"打开存档错误","非法的存档",QMessageBox::Ok);
            return;
        }
            for (int i=0;i<15;i++)
                for (int j=0;j<15;j++){
                    stream>>gameInfo->map[i][j];
                    qDebug()<<i<<" "<<j<<" "<<gameInfo->map[i][j];
                }
        isLoad=1;
        file->close();
        tcp->sendInfo("map",makeMap());
        talk("您载入了存档: "+fileDir);
    }else
    {
        qDebug()<<"no file in dir:"<<fileDir;
    }
}


void GameInterface::on_loadGame_button_clicked()
{
    if (isStart){
        talk("游戏已经开始,不能载入存档");
        return;
    }
    music->playMusic("menuclick");
    QUrl url = QFileDialog::getOpenFileUrl(this,"打开游戏存档",QUrl("/"),"(*.bak)");
    qDebug()<<"open file: "<<url.path();
    openBackup(url.toLocalFile());

}
