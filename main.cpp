#include "gameinterface.h"
#include "music.h"
#include "switcher.h"
#include "tcp.h"
#include "serverlistbox.h"
#include "welcomeinterface.h"
#include "waitingdialog.h"
#include "waitingdialog2.h"
#include <QApplication>
#include <QTextEdit>
#include <ctime>


GameInfo *gameInfo;
Music *music;
Switcher *switcher;
GameInterface *gameI;
WelcomeInterface *welcomeInterface;
ServerListBox *serverListBox;
Tcp *tcp;
QTextEdit *debugWindow;
WaitingDialog *waitingDialog;
WaitingDialog2 *waitingDialog2;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(NULL));
    music = new Music;
    music->addMusic("../chess/music/winsound.wav","win");
    music->addMusic("../chess/music/failsound.wav","fail");
    switcher = new Switcher;
    debugWindow =new QTextEdit;
    makeDebugWindow();

    waitingDialog = new WaitingDialog;
    waitingDialog2 = new WaitingDialog2;
    //waitingDialog->show();

    //if (!waitingDialog->isHidden()) waitingDialog->hide();

    tcp = new Tcp;
    gameInfo = new GameInfo;
    gameI = new GameInterface;
    welcomeInterface = new WelcomeInterface;
    serverListBox = new ServerListBox;

    switcher->add("game",gameI);
    switcher->add("welcome",welcomeInterface);

    switcher->showInterface("welcome");

    return a.exec();
}
