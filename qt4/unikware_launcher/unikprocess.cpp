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
  if (!output.startsWith("info: no tasks"))
    qDebug() << output;

  return output.startsWith(QString("\"%1\"").arg(proc));
}

/**
 * Run program
 *
 * @param proc process name
 * @param path of target program
 */
bool UnikProcess::exec(QString proc, QString path)
{
  QProcess process;

//  if (path.length() > 0) {
//    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//    env.insert("PATH", path + ";" + env.value("Path"));
//    process.setProcessEnvironment(env);
//  }
  int i = process.startDetached(proc);
  return i == 1;
}
