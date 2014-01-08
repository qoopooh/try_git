#include <QtWidgets>
#include "chat.h"

const int WAIT_FOR_READY(3000);

Chat::Chat(QWidget *parent, QString url, int port, QString id) :
    QDialog(parent), f_show_tx(true), uuid(id), phoneid(0)
{
  createHorizontalGroupBox();
//  createGridGroupBox();

  QVBoxLayout *mainLayout = new QVBoxLayout;

  mainLayout->addWidget(horizontalGroupBox, 1);
//  mainLayout->addWidget(gridGroupBox, 10);
  textEditLog = new QTextEdit();
  textEditLog->setReadOnly(true);
  mainLayout->addWidget(textEditLog, 10);
  setLayout(mainLayout);

  setWindowTitle(APP_TITLE + " " + VERSION);
#ifdef Q_OS_ANDROID
  showFullScreen();
#endif
  socket = new QTcpSocket();
  socket->connectToHost(url, port);
  stream = new QTextStream(socket);

  connect(socket, SIGNAL(connected()), this, SLOT(onStart()));
  connect(btnSend, SIGNAL(clicked()), this, SLOT(onSend()));
  connect(btnClear, SIGNAL(clicked()), this, SLOT(onClear()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(onRead()));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(onEror(QAbstractSocket::SocketError)));

  if ((uuid == NULL) || (uuid.length() != 40))
    uuid = UUID;
}

Chat::~Chat()
{
  delete socket;
}

void Chat::createHorizontalGroupBox()
{
  horizontalGroupBox = new QGroupBox(APP + " " + VERSION);
  QHBoxLayout *layout = new QHBoxLayout;
  lineEditCmd = new QLineEdit(tr("E,L,1"));
  btnSend = new QPushButton(tr("&Send"));
  btnClear = new QPushButton(tr("&Clear"));

  layout->addWidget(lineEditCmd, 4);
  layout->addWidget(btnSend, 1);
  layout->addWidget(btnClear, 1);

  horizontalGroupBox->setLayout(layout);
  horizontalGroupBox->setDisabled(true);
}

void Chat::createGridGroupBox()
{
  gridGroupBox = new QGroupBox(tr("Grid layout"));
  QGridLayout *layout = new QGridLayout;

  for (int i = 0; i < NumGridRows; ++i) {
    labels[i] = new QLabel(tr("Line %1:").arg(i + 1));
    lineEdits[i] = new QLineEdit;
    layout->addWidget(labels[i], i + 1, 0);
    layout->addWidget(lineEdits[i], i + 1, 1);
  }
  gridGroupBox->setLayout(layout);
}

void Chat::onStart()
{
  horizontalGroupBox->setDisabled(false);
  btnClear->click();
}

void Chat::onSend()
{
  QString data(lineEditCmd->text());
  GatewayMessage gMsg(uuid);

  if (f_show_tx) {
    QTime local(QTime::currentTime());
    QString str = local.toString("m:ss.zzz: ") + data + "\n";
    log(str);
  }

  data = gMsg.encrypt(data, phoneid);
  socket->write(data.toStdString().c_str(), data.length());
//  *stream << data.toStdString().c_str(); /* does not work */
}

void Chat::onClear()
{
  QString msg = GatewayMessage(uuid).registerMessage();
  socket->write(msg.toStdString().c_str(), msg.length());

  textEditLog->clear();
}

void Chat::onRead()
{
  GatewayMessage gMsg(uuid, stream->readLine());
  QString msg = gMsg.getDecryption();

  handleMessage(msg.split(','));
  QTime local(QTime::currentTime());
  QString str = local.toString("m:ss.zzz") + "-> " + msg;
  log(str);
}

void Chat::onEror(QAbstractSocket::SocketError e)
{
  QMessageBox::critical(this, APP,
                        QString("Connection Error: %1").arg(e),
                        QMessageBox::Yes);
  this->close();
}

void Chat::log(QString msg)
{
  QTextCursor tmpCursor = textEditLog->textCursor();

  tmpCursor.setPosition(0);
  textEditLog->setTextCursor(tmpCursor);
  textEditLog->insertPlainText(msg);
}

void Chat::handleMessage(QStringList list)
{
  char msg_type = list[1].at(0).toLatin1();
  char cmd = list[2].at(0).toLatin1();

  switch (msg_type) {
  case 'C':
    if (cmd == 'U') {
      phoneid = list[3].toInt();
    }
    break;
  default:
    break;
  }
}
