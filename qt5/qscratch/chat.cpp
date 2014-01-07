#include <QtWidgets>
#include "chat.h"

Chat::Chat(QWidget *parent) :
    QDialog(parent)
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
//  showFullScreen();
  socket = new QTcpSocket();
  socket->connectToHost("192.168.1.39", 1470);
  stream = new QTextStream(socket);

  connect(btnSend, SIGNAL(clicked()), this, SLOT(onSend()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(onRead()));
}

Chat::~Chat()
{
  delete socket;
}

void Chat::onSend()
{
  QString data(lineEditCmd->text());

  data += "\r\n";
  socket->write(data.toStdString().c_str(), data.length());
//  *stream << lineEditCmd->text() << "\r\n";
}

void Chat::onRead()
{
  QString resp(stream->readLine());

  textEditLog->append(resp);
}

void Chat::createHorizontalGroupBox()
{
  horizontalGroupBox = new QGroupBox(tr("Message"));
  QHBoxLayout *layout = new QHBoxLayout;
  lineEditCmd = new QLineEdit(tr("0,E,V"));
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

