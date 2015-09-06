#ifndef RECORD_H
#define RECORD_H

#include <QDateTime>

class Record
{
public:
    Record();
    int record[250][15][15];
    QString start,end;
    int step;
};

#endif // RECORD_H
