#ifndef CHAT_H
#define CHAT_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>
#include <QSettings>

#include "def.h"
#include "gatewaymessage.h"

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;

class Chat : public QDialog
{
  Q_OBJECT
public:
  explicit Chat(QWidget *parent = 0, QString url="192.168.1.33", int port=1470,
                QString id="");
  ~Chat();

signals:

public slots:

private slots:
  void onStart();
  void onSend();
  void onClear();
  void onRead();
  void onEror(QAbstractSocket::SocketError);

private:
  void createHorizontalGroupBox();
  void createGridGroupBox();
  void log(QString);
  void handleMessage(QStringList list);

  enum { NumGridRows = 3, NumButtons = 4 };

  QGroupBox *horizontalGroupBox;
  QGroupBox *gridGroupBox;
  QGroupBox *formGroupBox;
  QTextEdit *smallEditor;
  QTextEdit *bigEditor;
  QLabel *labels[NumGridRows];
  QLineEdit *lineEdits[NumGridRows];
  QPushButton *buttons[NumButtons];

  QLineEdit *lineEditCmd;
  QPushButton *btnSend;
  QPushButton *btnClear;
  QTextEdit *textEditLog;

  QTcpSocket *socket;
  QTextStream *stream;
  bool f_show_tx;
  QString uuid;
  int phoneid;
};

#endif // CHAT_H
