#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_checkBoxConnect_toggled(bool checked)
{
  if (checked) {
    quint16 port = ui->lineEditPort->text().toUInt();
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onRead()));
    tcpSocket->connectToHost(ui->lineEditIp->text(), port);
  } else {
    tcpSocket->disconnect();
    disconnect(tcpSocket, SIGNAL(connected()), this, SLOT(onConnect()));
    disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onRead()));
    delete tcpSocket;
  }
}

void MainWindow::onConnect()
{
  log("Connected");
}

void MainWindow::onRead()
{
  QByteArray ba = tcpSocket->readLine();

  qDebug() << ba.length() << ":" << ba.toHex();
  log(ba.constData());
}

void MainWindow::log(QString str)
{
  ui->textEditLog->append(str);
}

