#ifndef TCP_H
#define TCP_H

#include <QTimer>
#include <QTcpSocket>
#include <QTcpServer>
using std::vector;

class Tcp:public QObject
{
    Q_OBJECT
public:
    Tcp();

    QTcpServer *server;
    QTcpSocket* socket;
    QString cache;
    int blockSize;
    void dealInfo(QString info);
    void sendInfo(QString name,int num);
    void sendInfo(QString name,int num1,int num2);
    void sendInfo(QString name,QString info);
    QTimer mytime;
    QTimer myheart;

signals:
    void tcpNewConnectionAdd();
    void tcpReceiveInfo(QString info);
    void tcpConnectSuccess();

public slots:
    void newConnection();
    void connectionLost();
    void sendInfo(QString info);
    void receiveInfo();
    void connectToIP(QString ip);
    void makeServer();
};

extern Tcp *tcp;
#endif // TCP_H
