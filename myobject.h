#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QString>
#include <QObject>
#include <QTextStream>

class MyObject:public QObject
{
    Q_OBJECT

signals:

public:
    MyObject();
    virtual QString toString()=0;
    virtual void receiveString(QTextStream &stream)=0;
};

#endif // MYOBJECT_H
