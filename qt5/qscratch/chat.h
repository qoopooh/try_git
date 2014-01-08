#ifndef CHAT_H
#define CHAT_H

#include <QDialog>
#include <QTcpSocket>
#include "gatewaymessage.h"

class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;

class Chat : public QDialog
{
  Q_OBJECT
public:
  explicit Chat(QWidget *parent = 0);
  ~Chat();

signals:

public slots:

private slots:
  void onSend();
  void onClear();
  void onRead();

private:
  void createHorizontalGroupBox();
  void createGridGroupBox();
  void log(QString);
  void handleMessage(QStringList list);

  enum { NumGridRows = 3, NumButtons = 4 };

  QMenuBar *menuBar;
  QGroupBox *horizontalGroupBox;
  QGroupBox *gridGroupBox;
  QGroupBox *formGroupBox;
  QTextEdit *smallEditor;
  QTextEdit *bigEditor;
  QLabel *labels[NumGridRows];
  QLineEdit *lineEdits[NumGridRows];
  QPushButton *buttons[NumButtons];
  QDialogButtonBox *buttonBox;

  QMenu *fileMenu;
  QAction *exitAction;

  QLineEdit *lineEditCmd;
  QPushButton *btnSend;
  QPushButton *btnClear;
  QTextEdit *textEditLog;

  QTcpSocket *socket;
  QTextStream *stream;
  bool f_show_tx;
  int phoneid;
};

#endif // CHAT_H
