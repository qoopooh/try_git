#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QProcess>

namespace Ui {
  class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = 0);
  ~Dialog();
  bool isRunning(QString);
  bool kill(QString);

signals:
  void result(QString);

private slots:
  void on_pushButtonCheckStatus_clicked();

private:
  Ui::Dialog *ui;
};

#endif // DIALOG_H
