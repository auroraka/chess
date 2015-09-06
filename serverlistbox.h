#ifndef SERVERLISTBOX_H
#define SERVERLISTBOX_H

//#include "client.h"
#include <QWidget>
#include <QListWidgetItem>
#include <vector>
using std::vector;

namespace Ui {
class ServerListBox;
}

class ServerListBox : public QWidget
{
    Q_OBJECT

public:
    explicit ServerListBox(QWidget *parent = 0);
    ~ServerListBox();
    void clear();

signals:
    void client_tcpConnectToIP(QString ip);

public slots:
    void addIP(QString ip,QString name);


private slots:
    void on_addServer_clicked();

    void on_joinGame_clicked();


private:
    Ui::ServerListBox *ui;
    vector<QListWidgetItem*> items;
};

extern ServerListBox *serverListBox;

#endif // SERVERLISTBOX_H
