#include "qextserialport.h"
#include "serialthread.h"
#include "dialog.h"
#include "ui_dialog.h"
#include <QtCore>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //! [0]
    onPortAddedOrRemoved();
    //make sure user can input their own port name!
    ui->portBox->setEditable(true);

    ui->baudRateBox->addItem("1200", BAUD1200);
    ui->baudRateBox->addItem("2400", BAUD2400);
    ui->baudRateBox->addItem("4800", BAUD4800);
    ui->baudRateBox->addItem("9600", BAUD9600);
    ui->baudRateBox->addItem("19200", BAUD19200);
    ui->baudRateBox->setCurrentIndex(3);

    ui->parityBox->addItem("NONE", PAR_NONE);
    ui->parityBox->addItem("ODD", PAR_ODD);
    ui->parityBox->addItem("EVEN", PAR_EVEN);

    ui->dataBitsBox->addItem("5", DATA_5);
    ui->dataBitsBox->addItem("6", DATA_6);
    ui->dataBitsBox->addItem("7", DATA_7);
    ui->dataBitsBox->addItem("8", DATA_8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->stopBitsBox->addItem("1", STOP_1);
    ui->stopBitsBox->addItem("2", STOP_2);

    ui->queryModeBox->addItem("Polling", QextSerialPort::Polling);
    ui->queryModeBox->addItem("EventDriven", QextSerialPort::EventDriven);
    //! [0]

    ui->led->turnOff();

    //timer = new QTimer(this);
    //timer->setInterval(40);
    serial = new SerialThread("ttyS3", this);

    connect(ui->portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
    connect(ui->openCloseButton, SIGNAL(clicked()), SLOT(onOpenCloseButtonClicked()));
    connect(ui->sendButton, SIGNAL(clicked()), SLOT(onSendButtonClicked()));
    connect(serial, SIGNAL(data(QString)), SLOT(onReadyRead(QString)));

    setWindowTitle(tr("QextSerialPort Demo"));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog::onPortNameChanged(const QString & /*name*/)
{
  //if (port->isOpen()) {
  //port->close();
        ui->led->turnOff();
        //}
}
//! [3]
void Dialog::onOpenCloseButtonClicked()
{
  //if (!port->isOpen()) {
  //port->setPortName(ui->portBox->currentText());
  //port->open(QIODevice::ReadWrite);
        qDebug() << "opening";
        //}
        //else {
        //port->close();
        //qDebug() << "closing";
        //}

    //If using polling mode, we need a QTimer
    //if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
    //timer->start();
    //else
    //timer->stop();

    //update led's status
    //ui->led->turnOn(port->isOpen());
}
//! [3]
//! [4]
void Dialog::onSendButtonClicked()
{
  //if (port->isOpen() && !ui->sendEdit->toPlainText().isEmpty()) {
  const char *data = ui->sendEdit->toPlainText().toLatin1();
  //port->write(data);
  //port->write("\r");
    qDebug() << data;
    //}
}

void Dialog::onReadyRead(const QString &data)
{
  ui->recvEdit->moveCursor(QTextCursor::End);
  ui->recvEdit->insertPlainText(data);
}

void Dialog::onPortAddedOrRemoved()
{
  QList<QString> ports = SerialThread::discovery();
  QString current = ui->portBox->currentText();

  ui->portBox->blockSignals(true);
  ui->portBox->clear();
  foreach (QString info, ports)
    ui->portBox->addItem(info);
  ui->portBox->setCurrentIndex(ui->portBox->count() - 1);
  ui->portBox->blockSignals(false);
}

//! [4]
