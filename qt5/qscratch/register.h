#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

#include "def.h"
#include "chat.h"

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;

class Register : public QDialog
{
  Q_OBJECT
public:
  explicit Register(QWidget *parent = 0);

signals:

private slots:
  void onConnect();

private:
  void createVerticalGroupBox();

  QGroupBox *verticalGroupBox;

  QLabel *labelUrl;
  QLabel *labelPort;
  QLabel *labelId;
  QLineEdit *lineEditUrl;
  QLineEdit *lineEditPort;
  QTextEdit *textEditId;
  QPushButton *btnConnect;
  QPushButton *btnBack;
};

#endif // REGISTER_H
