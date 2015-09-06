#ifndef ENVIROMENT
#define ENVIROMENT

#include <QString>
#include <QHostInfo>
#include <QTextEdit>

const int TcpListenPort=10086;
const int UdpListenPort=12345;
const int GameSize=15;
const int BlockLen=35;
const int StartX=22,StartY=22;
const int BackgroundLen=535;

QString getIP();

QString getHostName();

bool checkIP(QString ip);

void SystemError(QWidget* obj=0);

extern QTextEdit *debugWindow;

void makeDebugWindow();
void Debug(QString info);

#endif // ENVIROMENT

