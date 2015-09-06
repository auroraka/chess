#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QDialog>

namespace Ui {
class WaitingDialog;
}

class WaitingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingDialog(QWidget *parent = 0);
    ~WaitingDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::WaitingDialog *ui;
};

extern WaitingDialog *waitingDialog;

#endif // WAITINGDIALOG_H
