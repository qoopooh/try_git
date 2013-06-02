#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include "serialthread.h"

namespace Ui {
    class Dialog;
}
class QTimer;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void sendData(QByteArray);

protected:
    void changeEvent(QEvent *e);

private Q_SLOTS:
    void onPortNameChanged(const QString &name);
    void onOpenCloseButtonClicked();
    void onSendButtonClicked();
    void onReadyRead(const QString &);

    void onPortAddedOrRemoved();

private:
    Ui::Dialog *ui;
    SerialThread serial;
};

#endif // DIALOG_H
