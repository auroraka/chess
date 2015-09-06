#include "enviroment.h"
#include <QHostInfo>
#include <QHostAddress>
#include <QMessageBox>
#include <QRegExp>

QString getIP(){
    QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address,info.addresses()){
        if (address.protocol()==QAbstractSocket::IPv4Protocol){
            QString ip=address.toString();
          //  qDebug()<<"server: get local ip: "<<ip;
            return ip;
        }
    }
}


QString getHostName(){
    return QHostInfo::localHostName();
}

void SystemError(QWidget* obj){
    QMessageBox::critical(obj,"发生了一点错误","对此我们感到非常抱歉",QMessageBox::YesAll);
    exit(0);
}


void makeDebugWindow(){
    debugWindow = new QTextEdit(0);
    debugWindow->setWindowTitle("Server debug info");
    debugWindow->resize(300,300);
    debugWindow->show();
}

void Debug(QString info){\
    if (debugWindow==NULL){
        qDebug()<<info;
        return;
    }
    debugWindow->append(info);
}
bool checkIP(QString ip){
    QRegExp rx2( "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
    return rx2.exactMatch(ip);
}

