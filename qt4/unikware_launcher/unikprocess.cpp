#include "unikprocess.h"

UnikProcess::UnikProcess(QObject *parent) :
  QObject(parent)
{
}

bool UnikProcess::isRunning(QString proc)
{
  QProcess tasklist;
  tasklist.start(
        "tasklist",
        QStringList() << "/NH"
                      << "/FO" << "CSV"
                      << "/FI" << QString("IMAGENAME eq %1").arg(proc));
  tasklist.waitForFinished();
  QString output = tasklist.readAllStandardOutput();
  return output.startsWith(QString("\"%1").arg(proc));
}

bool UnikProcess::exec(QString proc)
{
  QProcess tasklist;
  int i = tasklist.startDetached(proc);
  qDebug() << proc << i;
  return i == 0;
}


