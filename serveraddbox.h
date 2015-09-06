#ifndef SERVERADDBOX_H
#define SERVERADDBOX_H

#include <QWidget>

namespace Ui {
class ServerAddBox;
}

class ServerAddBox : public QWidget
{
    Q_OBJECT

public:
    explicit ServerAddBox(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *event);


    ~ServerAddBox();
public slots:
    void textAdd(QString a);
signals:
    void finished(QString ip);
private:
    Ui::ServerAddBox *ui;
    QString myIP;
};

#endif // SERVERADDBOX_H
