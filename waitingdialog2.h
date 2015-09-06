#ifndef WAITINGDIALOG2_H
#define WAITINGDIALOG2_H

#include <QDialog>

namespace Ui {
class WaitingDialog2;
}

class WaitingDialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingDialog2(QWidget *parent = 0);
    ~WaitingDialog2();

private slots:
    void on_pushButton_clicked();

private:
    Ui::WaitingDialog2 *ui;
};

extern WaitingDialog2 *waitingDialog2;

#endif // WAITINGDIALOG_H
