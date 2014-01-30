#include <QtWidgets>
#include "chat.h"

const int WAIT_FOR_READY(3000);
const QString UNIT_ID("0123456789012345678901234567890123456789");

Chat::Chat(QWidget *parent, QString url, int port, QString id) :
    QDialog(parent), f_show_tx(true), f_send_cmd(false), uuid(id), phoneid(0)
{
  createHorizontalGroupBox();

  textEditLog = new QTextEdit(this);
//  textEditLog->setReadOnly(true);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(horizontalGroupBox, 1);
  mainLayout->addWidget(textEditLog, 10);
  setLayout(mainLayout);
  setWindowTitle(APP_TITLE + " " + VERSION);

#ifdef Q_OS_ANDROID
  showFullScreen();
#endif

  socket = new QTcpSocket(this);
  socket->connectToHost(url, port);

  connect(socket, SIGNAL(connected()), this, SLOT(onStart()));
  connect(btnSend, SIGNAL(clicked()), this, SLOT(onSend()));
  connect(btnClear, SIGNAL(clicked()), this, SLOT(onClear()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(onRead()));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(onEror(QAbstractSocket::SocketError)));

  if ((uuid == NULL) || (uuid.length() != 40))
    uuid = UNIT_ID;

  QSettings settings;
  QString cmd = settings.value("cmd").toString();
  if (cmd.length() < 1)
    return;
  lineEditCmd->setText(cmd);
}

Chat::~Chat()
{
  delete socket;
}

void Chat::createHorizontalGroupBox()
{
  horizontalGroupBox = new QGroupBox(APP + " " + VERSION, this);
  QHBoxLayout *layout = new QHBoxLayout(this);
  lineEditCmd = new QLineEdit(tr("E,L,1"), this);
  btnSend = new QPushButton(tr("&Send"), this);
  btnClear = new QPushButton(tr("&Clear"), this);

#ifdef Q_OS_IOS
  btnBack = new QPushButton(tr("<-"), this);
  btnBack->setFlat(true);
  btnBack->setMaximumWidth(20);
  layout->addWidget(btnBack);
  connect(btnBack, SIGNAL(clicked()), this, SLOT(onBack()));
#endif

  layout->addWidget(lineEditCmd, 8);
  layout->addWidget(btnSend, 2);
  layout->addWidget(btnClear, 2);

  horizontalGroupBox->setLayout(layout);
  horizontalGroupBox->setDisabled(true);
}

void Chat::onStart()
{
  QString msg = GatewayMessage(uuid).registerMessage();
  socket->write(msg.toStdString().c_str(), msg.length());

  horizontalGroupBox->setDisabled(false);
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
  f_send_cmd = true;
}

void Chat::onClear()
{
  textEditLog->clear();
}

void Chat::onRead()
{
  while (socket->canReadLine()) {
    GatewayMessage gMsg(uuid, socket->readLine());
    QString msg = gMsg.getDecryption();

    handleMessage(msg.split(','));
    QTime local(QTime::currentTime());
    QString str = local.toString("m:ss.zzz") + "-> " + msg;
    log(str);
  }

  if (f_send_cmd) {
    QSettings settings;
    settings.setValue("cmd", lineEditCmd->text());
    f_send_cmd = false;
  }
}

void Chat::onBack()
{
  this->close();
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
