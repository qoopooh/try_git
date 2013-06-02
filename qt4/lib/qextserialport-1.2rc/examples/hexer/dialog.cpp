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

    //! [0]

    ui->led->turnOff();

    //timer = new QTimer(this);
    //timer->setInterval(40);
    //serial = new SerialThread();

    connect(ui->portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
    connect(ui->openCloseButton, SIGNAL(clicked()), SLOT(onOpenCloseButtonClicked()));
    connect(ui->sendButton, SIGNAL(clicked()), SLOT(onSendButtonClicked()));
    connect(&serial, SIGNAL(data(QString)), SLOT(onReadyRead(QString)));
    connect(this, SIGNAL(sendData(QByteArray)), &serial, SLOT(write(QByteArray)));

    setWindowTitle(tr("QextSerialPort Demo"));
}

Dialog::~Dialog()
{
  if (serial.isRunning()) {
    serial.quit();
    serial.wait();
  }
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

void Dialog::onPortNameChanged(const QString &name)
{
  serial.setPort(name);
  if (serial.isRunning()) {
    serial.quit();
    ui->led->turnOff();
  }
}
//! [3]
void Dialog::onOpenCloseButtonClicked()
{
  if (!serial.isRunning()) {
    //port->setPortName(ui->portBox->currentText());
    //port->open(QIODevice::ReadWrite);
    qDebug() << "opening";
    onPortNameChanged(ui->portBox->currentText());
    serial.start();
  } else {
    qDebug() << "closing";
    serial.quit();
    serial.wait();
  }

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
  emit sendData(data);
  emit sendData("\r");
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
