#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

#include "def.h"
#ifndef FTC
#include "chat.h"
#else
#endif


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
  QLineEdit *lineEditUrl;
  QLineEdit *lineEditPort;
#ifndef FTC
  QLabel *labelId;
  QTextEdit *textEditId;
#endif
  QPushButton *btnConnect;
  QPushButton *btnBack;
};

#endif // REGISTER_H
