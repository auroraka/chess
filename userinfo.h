#ifndef USERINFO_H
#define USERINFO_H

#include "myobject.h"

enum UserStatus{Waiting,Prepare,Gaming,MyTurn};

class UserInfo
{
public:
    UserInfo();
    QString toString();
    void receiveString(QTextStream &stream);

public:
    QString ip;
    QString name;
    int nameListTot=4;
    int userID;
    UserStatus status;
    int getAllInfoo;
    int color;
};

extern UserInfo *myuser;
#endif // USERINFO_H
