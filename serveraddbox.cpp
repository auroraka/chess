#include "serveraddbox.h"
#include "ui_serveraddbox.h"
#include "music.h"
#include <QKeyEvent>
#include <QSignalMapper>
#include <QDebug>

ServerAddBox::ServerAddBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerAddBox)
{
    ui->setupUi(this);

    myIP="";
    QSignalMapper *m = new QSignalMapper(this);

    //set 0
    connect(ui->pushButton_0,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_0,0);
    //set 1
    connect(ui->pushButton_1,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_1,"1");
    //set 2
    connect(ui->pushButton_2,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_2,"2");
    //set 3
    connect(ui->pushButton_3,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_3,"3");
    //set 4
    connect(ui->pushButton_4,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_4,"4");
    //set 5
    connect(ui->pushButton_5,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_5,"5");
    //set 6
    connect(ui->pushButton_6,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_6,"6");
    //set 7
    connect(ui->pushButton_7,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_7,"7");
    //set 8
    connect(ui->pushButton_8,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_8,"8");
    //set 9
    connect(ui->pushButton_9,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_9,"9");
    //set .
    connect(ui->pushButton_point,SIGNAL(clicked()),m,SLOT(map()));
    m->setMapping(ui->pushButton_point,".");

    //关联号码按键和文字显示
    connect(m,SIGNAL(mapped(QString)),this,SLOT(textAdd(QString)));
    //关联清除按键和文字清除
    connect(ui->pushButton_cancel,&QPushButton::clicked,[=](){
        music->playMusic("menuclick");
        myIP="";
        ui->textEdit->clear();
    });

    connect(ui->pushButton_add,&QPushButton::clicked,[=](){
        music->playMusic("menuclick");
        emit finished(ui->textEdit->toPlainText());
        close();
    });
}

void ServerAddBox::keyPressEvent(QKeyEvent *event){
    qDebug()<<"key press:"<<event->text();
    for (int i=0;i<9;i++){
        if (event->text()==QString::number(i)){
            myIP+=QString::number(i);
        }
    }
    if (event->text()=="."){
        myIP+=".";
    }
    if (event->text()=="\b"){
        if (myIP!=""){
            myIP.remove(myIP.size()-1,1);
        }
    }
    if (event->text()=="\r"){
        emit finished(ui->textEdit->toPlainText());
        close();
    }
    ui->textEdit->clear();
    ui->textEdit->append(myIP);
}

void ServerAddBox::textAdd(QString a){
    myIP+=a;
    ui->textEdit->clear();
    ui->textEdit->append(myIP);
}

ServerAddBox::~ServerAddBox()
{
    delete ui;
}
