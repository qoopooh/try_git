#include "unikprocess.h"

UnikProcess::UnikProcess(QObject *parent) :
  QObject(parent)
{
}

bool UnikProcess::isRunning(QString proc)
{
  QProcess tasklist;
  QStringList args(QStringList() << "/NH"
                  << "/FO" << "CSV"
                  << "/FI" << QString("IMAGENAME eq %1").arg(proc));
  tasklist.start("tasklist", args);
  tasklist.waitForFinished();
  QString output = tasklist.readAllStandardOutput().trimmed().toLower();
  qDebug() << output;
  return output.startsWith(QString("\"%1\"").arg(proc));
}

bool UnikProcess::exec(QString proc)
{
  QProcess tasklist;
  int i = tasklist.startDetached(proc);
//  qDebug() << proc << i;
  return i == 1;
}
