#ifndef WELCOMEINTERFACE_H
#define WELCOMEINTERFACE_H

//#include "client.h"
#include <QWidget>

namespace Ui {
class WelcomeInterface;
}

class WelcomeInterface : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeInterface(QWidget *parent = 0);
    ~WelcomeInterface();
    void openBackup(QString fileDir);


private slots:
    void on_createGame_button_clicked();

    void on_addGame_button_clicked();

    void on_onePersonGame_button_clicked();


    void on_rename_button_clicked();

private:
    Ui::WelcomeInterface *ui;
};

#endif // WELCOMEINTERFACE_H
