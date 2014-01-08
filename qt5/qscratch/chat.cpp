#include <QtWidgets>
#include "chat.h"

const QString APP("Domotics Gateway");
const QString VERSION("V0.1");
const QString UUID("0123456789012345678901234567890123456789");

Chat::Chat(QWidget *parent) :
    QDialog(parent), f_show_tx(true), phoneid(0)
{
  createHorizontalGroupBox();
//  createGridGroupBox();

  QVBoxLayout *mainLayout = new QVBoxLayout;

  mainLayout->addWidget(horizontalGroupBox, 1);
//  mainLayout->addWidget(gridGroupBox, 10);
  textEditLog = new QTextEdit();
  mainLayout->addWidget(textEditLog, 10);
  setLayout(mainLayout);

  setWindowTitle(tr("Basic Layouts"));
#ifdef Q_OS_ANDROID
  showFullScreen();
#endif
  socket = new QTcpSocket();
  socket->connectToHost("192.168.1.32", 1470);
  stream = new QTextStream(socket);

  connect(btnSend, SIGNAL(clicked()), this, SLOT(onSend()));
  connect(btnClear, SIGNAL(clicked()), this, SLOT(onClear()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(onRead()));
}

Chat::~Chat()
{
  delete socket;
}

void Chat::onSend()
{
  QString data(lineEditCmd->text());
  GatewayMessage gMsg(UUID);

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
  QString msg = GatewayMessage(UUID).registerMessage();
  socket->write(msg.toStdString().c_str(), msg.length());

  textEditLog->clear();
}

void Chat::onRead()
{
  GatewayMessage gMsg(UUID, stream->readLine());
  QString msg = gMsg.getDecryption();

  handleMessage(msg.split(','));
  QTime local(QTime::currentTime());
  QString str = local.toString("m:ss.zzz") + "-> " + msg;
  log(str);
}

void Chat::createHorizontalGroupBox()
{
  horizontalGroupBox = new QGroupBox(APP + " " + VERSION);
  QHBoxLayout *layout = new QHBoxLayout;
  lineEditCmd = new QLineEdit(tr("E,L,1"));
  btnSend = new QPushButton(tr("Send"));
  btnClear = new QPushButton(tr("Clear"));

  layout->addWidget(lineEditCmd, 4);
  layout->addWidget(btnSend, 1);
  layout->addWidget(btnClear, 1);

  horizontalGroupBox->setLayout(layout);
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
