#include "waitingdialog.h"
#include "enviroment.h"
#include "ui_waitingdialog.h"
#include "music.h"
#include "tcp.h"

WaitingDialog::WaitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDialog)
{
    ui->setupUi(this);
    ui->label->setText("正在等待对方连接\n你的IP是: "+getIP());

}

WaitingDialog::~WaitingDialog()
{
    delete ui;
}

void WaitingDialog::on_pushButton_clicked()
{
    music->playMusic("menuback");
    tcp->server->close();
    waitingDialog->hide();
}
