#ifndef SWITCHER_H
#define SWITCHER_H

#include <QString>
#include <QWidget>
#include <map>
using std::string;
using std::map;

class Switcher
{
public:
    Switcher();
    void add(QString name,QWidget *interface);
    void showInterface(QString name,QString mode="menuclick");
private:
   map<QString,QWidget*> switcher;
   QWidget *now;
};


extern Switcher *switcher;
#endif // SWITCHER_H
