#include "welcomeinterface.h"
#include "ui_welcomeinterface.h"
#include "gameinterface.h"
#include "serverlistbox.h"
#include "tcp.h"
#include "music.h"
#include "switcher.h"
#include "waitingdialog.h"
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QInputDialog>
#include <QGraphicsScene>
#include <QGraphicsView>

WelcomeInterface::WelcomeInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeInterface)
{
    ui->setupUi(this);
    ui->onePersonGame_button->hide();
    ui->label_2->setText(gameI->userName);
    //添加背景
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("../chess/picture/bg2.jpg")));
    this->setPalette(palette);
    setAcceptDrops(true);

    //设置不透明度
   // setWindowOpacity(0.95);

}

WelcomeInterface::~WelcomeInterface()
{
    delete ui;
}

void WelcomeInterface::on_createGame_button_clicked()
{

    music->playMusic("menuclick");
    waitingDialog->show();
    tcp->makeServer();
}

void WelcomeInterface::on_addGame_button_clicked()
{
    music->playMusic("menuclick");
    serverListBox->clear();
    serverListBox->show();
}

void WelcomeInterface::on_onePersonGame_button_clicked()
{
    music->playMusic("menuclick");
    switcher->showInterface("game");
}



void WelcomeInterface::on_rename_button_clicked()
{
    QString name=QInputDialog::getText(this,"更改名字","请输入新游戏名称");
    gameI->userName=name;
    ui->label_2->setText(gameI->userName);
}
