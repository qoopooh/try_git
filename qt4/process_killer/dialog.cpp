#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
  ui->setupUi(this);
  connect(this,SIGNAL(result(QString)),ui->textEditResult,SLOT(setText(QString)));
}

Dialog::~Dialog()
{
  delete ui;
}

void Dialog::on_pushButtonCheckStatus_clicked()
{
  QString process(ui->lineEditProcessName->text());
  bool running(isRunning(process));
  qDebug() << "running" << running;
  if (running) {
    qDebug() << "killed" << kill(process);
  }
}

bool Dialog::isRunning(QString name)
{
  QProcess tasklist;
  tasklist.start(
        "tasklist",
        QStringList() << "/NH"
                      << "/FO" << "CSV"
                      << "/FI" << QString("IMAGENAME eq %1").arg(name));
  tasklist.waitForFinished();
  QString output = tasklist.readAllStandardOutput();
  qDebug() << output;
  emit result(output);
  return output.startsWith(QString("\"%1").arg(name));
}

bool Dialog::kill(QString name)
{
  QProcess taskkill;
  taskkill.start(
        "taskkill",
        QStringList() << "/F"
                      << "/IM" << name);
  taskkill.waitForFinished();
  QString output = taskkill.readAllStandardOutput();
  qDebug() << output;
  emit result(output);
  return output.startsWith("SUCCESS");
}
