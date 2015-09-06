#include "tcp.h"
#include "enviroment.h"
#include "gameinterface.h"
#include "switcher.h"
#include "waitingdialog.h"
#include "waitingdialog2.h"
#include <QString>
#include <QMessageBox>


Tcp::Tcp()
{
    cache="";
    server = new QTcpServer;
    QObject::connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

void Tcp::makeServer(){
    server->listen(QHostAddress::Any,TcpListenPort);
    Debug("listening...");
}


void Tcp::connectToIP(QString ip){  
    QTcpSocket *newSocket  = new QTcpSocket;
    socket=newSocket;
    cache="";
    newSocket->connectToHost(ip,10086);

    waitingDialog2->show();
    //newSocket->connectToHost(ip,TcpListenPort);

    //SIGNAL(error(QAbstractSocket::SocketError)
//    connect(socket, &QTcpSocket::error,[=](){
//        Debug("[ERROR] connection error");
//    });

    connect(newSocket,&QTcpSocket::connected,[=](){
        Debug("connection success");
        waitingDialog2->hide();
        connect(&gameI->heartBeat,&QTimer::timeout,[=](){
            Debug("check heart...");
            sendInfo("heartbeat");
            myheart.start(5*1000);
        });
        gameI->heartBeat.start(5*1000);

        connect(&mytime,&QTimer::timeout,[=](){
            gameI->saveNow("connectionLost.bak");
            QMessageBox::critical(0,"糟糕,对方连接好像断开了呢","您的游戏已经保存至connectionLost.bak,请重新连接再来吧",QMessageBox::Yes);
            exit(1);

        });
        connect(&myheart,&QTimer::timeout,[=](){
            gameI->saveNow("connectionLost.bak");
            QMessageBox::critical(0,"糟糕,对方连接好像断开了呢","您的游戏已经保存至connectionLost.bak,请重新连接再来吧",QMessageBox::Yes);
            exit(2);
        });



        //emit tcpConnectSuccess();
    });
    connect(newSocket,&QTcpSocket::readyRead,[=](){
        receiveInfo();
    });


}


void Tcp::newConnection(){
    //debugWindow->append("server: new connection request");

    QTcpSocket *newSocket=server->nextPendingConnection();

    //debugWindow->append("server: new socket create");

    socket=newSocket;
    cache="";

    connect(newSocket,&(newSocket->readyRead),[=](){
        receiveInfo();
    });
    connect(newSocket,&(newSocket->disconnected),[=]{
        connectionLost();
    });

    Debug("new connection");

    connect(&gameI->heartBeat,&QTimer::timeout,[=](){
        Debug("check heart...");
        sendInfo("heartbeat");
        myheart.start(5*1000);
    });
    gameI->heartBeat.start(5*1000);

    connect(&mytime,&QTimer::timeout,[=](){
        gameI->saveNow("connectionLost.bak");
        QMessageBox::critical(0,"糟糕,对方连接好像断开了呢","您的游戏已经保存至connectionLost.bak,请重新连接再来吧",QMessageBox::Yes);
        exit(1);

    });
    connect(&myheart,&QTimer::timeout,[=](){
        gameI->saveNow("connectionLost.bak");
        QMessageBox::critical(0,"糟糕,对方连接好像断开了呢","您的游戏已经保存至connectionLost.bak,请重新连接再来吧",QMessageBox::Yes);
        exit(2);
    });



    sendInfo("connect_success");
    sendInfo("my_color",gameI->myColor);
    sendInfo("now_color",gameI->nowColor);
    waitingDialog->hide();
    switcher->showInterface("game");

    emit tcpNewConnectionAdd();
}



void Tcp::connectionLost(){
    //more...
}
void Tcp::receiveInfo()
{
    //Debug("tcp receive :"+QString::number(cache.size()));
    while (socket->canReadLine()){
        QString info=socket->readLine();
        if (info.contains("__START__")){
            mytime.start(3*1000);
            cache="";
        }else if (info.contains("__END__")){
            mytime.stop();
            //emit tcpReceiveInfo(cache);
            dealInfo(cache);
            Debug("tcp receive info:"+cache);
//            package->packageReceive(cache[now]); !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            cache="";
        }else{
            cache+=info;
        }
    }

}

void Tcp::sendInfo(QString info){
    Debug("send info: "+info);
    info.replace(" ","@@");
    info.replace("\n","##");
    QString str=info;
    QTextStream(&info)<<"\n\n__START__\n\n"<<str<<"\n\n__END__\n\n";
    QByteArray byte;
    byte.append(info);
    socket->write(byte.data());

}

void Tcp::sendInfo(QString name,int num){
    QString info;
    QTextStream(&info)<<"\n"<<name<<"\n"<<num<<"\n";
    sendInfo(info);
}
void Tcp::sendInfo(QString name, int num1,int num2){
    QString info;
    QTextStream(&info)<<"\n"<<name<<"\n"<<num1<<"\n"<<num2<<"\n";
    tcp->sendInfo(info);
}
void Tcp::sendInfo(QString name,QString info){
    info.replace(" ","_@");
    info.replace("\n","_#");
    QString str=info;
    info="";
    QTextStream(&info)<<"\n"<<name<<"\n"<<str<<"\n";
    Debug("send talk: "+info);
    tcp->sendInfo(info);
}


void Tcp::dealInfo(QString info){
    info.trimmed();
    info.replace("@@"," ");
    info.replace("##","\n");
    Debug("receive: "+info);

    QTextStream stream(&info);
    QString head;
    stream>>head;
    if (head=="my_color"){
        int color;
        stream>>color;
        gameI->myColor =(color^1);
    }else if (head=="move_one_step"){
        int x,y;
        stream>>x>>y;
        gameI->moveOneStep(x,y);
    }else if (head=="now_color"){
        int color;
        stream>>color;
        gameI->nowColor=color;
    }else if (head=="prepare"){
        QString name;
        stream>>name;
        gameI->prepareAdd(name);
    }else if (head=="talk"){
        QString info;
        stream>>info;
        Debug("receive talk: "+info);
        info.replace("_@"," ");
        info.replace("_#","\n");
        info=info.remove(info.size()-1,1);
        gameI->talk(info+"\n");
    }else if (head == "exit_request"){
        int ret=QMessageBox::question(0,"有新消息","对方想要退出,请问您同意吗?",QMessageBox::Yes,QMessageBox::No);
        if (ret==QMessageBox::Yes){
            sendInfo("reply_exit_request",1);
            QMessageBox::information(0,"您同意了对方的退出","那么再见了...",QMessageBox::YesAll);
            exit(0);
        }else{
            sendInfo("reply_exit_request",0);
        }
    }else if (head=="reply_exit_request"){
        int reply;
        stream>>reply;
        if (reply==0){
            QMessageBox::information(0,"糟糕","对方还想和你继续呢...",QMessageBox::YesAll);
        }else{
            QMessageBox::information(0,"对方同意了","那么再见了...",QMessageBox::YesAll);
            exit(0);
        }
    }else if (head=="regret_request"){
        int ret=QMessageBox::question(0,"有新消息","对方想要反悔,请问您同意吗?",QMessageBox::Yes,QMessageBox::No);
        if (ret==QMessageBox::Yes){
            sendInfo("reply_regret_request",1);
            gameI->talk("您同意了对方的反悔");
            gameI->regretEvent();
            gameI->totRegret++;
        }else{
            sendInfo("reply_regret_request",0);
            gameI->talk("您没有同意对方的反悔");
        }
    }else if (head=="reply_regret_request"){
        int reply;
        stream>>reply;
        if (reply==0){
            QMessageBox::information(0,"糟糕","对方不同意反悔呢...",QMessageBox::YesAll);
        }else{
            gameI->talk("对方同意了您的反悔");
            gameI->regretEvent();
        }
    }else if (head=="map"){
        QString info;
        stream>>info;
        info.replace("_@"," ");
        info.replace("_#","\n");
        QTextStream sm(&info);
        for (int i=0;i<15;i++)
            for (int j=0;j<15;j++){
                sm>>gameInfo->map[i][j];
                qDebug()<<"receive "<<i<<" "<<j<<" "<<gameInfo->map[i][j];
            }
        gameI->myUpdate();
        gameI->talk("对方载入了存档");
    }else if (head=="connect_success"){
        switcher->showInterface("game");
    }else if (head=="heartbeat"){
        Debug("heart reply");
        sendInfo("alive");
    }else if (head=="alive"){
        Debug("heart checked.");
        myheart.stop();
    }
}
