#include "userinfo.h"
#include "enviroment.h"

#include <QTextStream>

const QString nameList[]={
    "Tom",
    "SillB",
    "XIXI",
    "OMG"
};

UserInfo::UserInfo()
{
    name=nameList[rand()%nameListTot];
    ip=getIP();
    status=Waiting;
    getAllInfoo=0;
    color=2;
}
//QString ip;
//QString name;
//int nameListTot=4;
//int userID;
//UserStatus status;

//int getAllInfoo;
//int color;

QString UserInfo::toString(){
    QString info;
    QTextStream stream(&info);
    stream<<"\n __UserInfo__\n";
    stream<<"\n ip\n"<<ip;
    stream<<"\n name\n"<<name;
    stream<<"\n nameListTot\n"<<nameListTot;
    stream<<"\n userID\n"<<userID;
    stream<<"\n status\n"<<status;
    stream<<"\n color\n"<<color;
    stream<<"\n __End__\n";
    return info;
}

void UserInfo::receiveString(QTextStream &stream){
  //  qDebug()<<"UserInfo->";
    while (stream.status() == QTextStream::Ok){
        QString oper;
        stream>>oper;
        if (oper=="__End__"){
        //    qDebug()<<"UserInfo:End->";
            break;
        }
        if (oper=="ip"){
            stream>>ip;
         //   qDebug()<<"ip: "<<ip;
        }
        if (oper=="name"){
            stream>>name;
         //   qDebug()<<"name: "<<name;
        }
        if (oper=="nameListTot"){
            stream>>nameListTot;
          //  qDebug()<<"nameListTot: "<<nameListTot;
        }
        if (oper=="userID"){
            stream>>userID;
          //  qDebug()<<"userID: "<<userID;
        }
        if (oper=="status"){
            int a;
            stream>>a;
            status=UserStatus(a);
           // qDebug()<<"status: "<<status;
        }
        if (oper=="color"){
            stream>>color;
        }
    }
}
