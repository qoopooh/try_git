#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
    class Dialog;
}
class QTimer;
class QextSerialPort;
class SerialThread;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

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
    QTimer *timer;
    QextSerialPort *port;
    SerialThread *serial;
};

#endif // DIALOG_H