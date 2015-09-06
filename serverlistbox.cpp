#include "serverlistbox.h"
#include "serveraddbox.h"
#include "ui_serverlistbox.h"
#include "enviroment.h"
#include "tcp.h"
#include "switcher.h"
#include "music.h"
#include <QDebug>
#include <QRegExp>

ServerListBox::ServerListBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerListBox)
{
    ui->setupUi(this);
    connect(ui->serverList,&QListWidget::itemClicked,[=](QListWidgetItem* item){
        ui->label->setText(item->text());
    });
    //connect(client,SIGNAL(findServer(QString,QString)),this,SLOT(addIP(QString,QString)));
}

void ServerListBox::clear(){
   items.clear();
}

ServerListBox::~ServerListBox()
{
    delete ui;
}

void ServerListBox::addIP(QString ip,QString name){
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(ip);
    items.push_back(item);
    //qDebug()<<"items tot: "<<items.size();
    ui->serverList->insertItem(items.size(),item);
    ui->serverList->setCurrentItem(item);
    qDebug()<<"added";

}

void ServerListBox::on_addServer_clicked()
{
    music->playMusic("menuclick");
    ServerAddBox *addBox = new ServerAddBox;
    connect(addBox,&ServerAddBox::finished,[=](QString ip){
        if (checkIP(ip)){
            addIP(ip,"");
            emit ui->serverList->itemClicked(items[items.size()-1]);
        }else{
            ui->label->setText("ip地址不合法");
        }
    });
    addBox->show();
}

void ServerListBox::on_joinGame_clicked()
{
    music->playMusic("menuclick");
    tcp->connectToIP(ui->label->text());
    //switcher->showInterface("game");
    hide();
}

