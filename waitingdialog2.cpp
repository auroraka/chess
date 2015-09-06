#include "waitingdialog2.h"
#include "enviroment.h"
#include "ui_waitingdialog2.h"
#include "music.h"
#include "tcp.h"

WaitingDialog2::WaitingDialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDialog2)
{
    ui->setupUi(this);
    ui->label->setText("正在等待连接\n请稍等...");
}

WaitingDialog2::~WaitingDialog2()
{
    delete ui;
}

void WaitingDialog2::on_pushButton_clicked()
{
    music->playMusic("menuback");
    tcp->socket->close();
    waitingDialog2->hide();
}
