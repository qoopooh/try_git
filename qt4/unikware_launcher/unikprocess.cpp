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

bool UnikProcess::exec(QString proc, QString path)
{
  QProcess process;

  if (path.length() > 0) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//    env.insert("TMPDIR", "C:\\MyApp\\temp"); // Add an environment variable
    env.insert("PATH", path + ";" + env.value("Path"));
    process.setProcessEnvironment(env);
//    qDebug() << process.environment();
  }
  int i = process.startDetached(proc);
//  qDebug() << proc << i;
  return i == 1;
}
