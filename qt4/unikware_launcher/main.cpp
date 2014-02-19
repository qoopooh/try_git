//#include <QtCore/QCoreApplication>
#include "unikprocess.h"

const QString UNIK_PROC("unikware.exe");
const QString UNIKMON_PROC("unikware_monitor.exe");
const QString PATH("N:\\Program\\");
const QString UNIKWARE = PATH + UNIK_PROC;
const QString UNIKWARE_MON = PATH + "UnikwareMonitor\\unikware_systray.exe";
int main(int argc, char *argv[])
{
  qDebug() << argv << argc;
//  QCoreApplication a(argc, argv);
  UnikProcess process;
  if (!process.isRunning(UNIKMON_PROC))
    process.exec(UNIKWARE_MON);
  process.exec(UNIKWARE);
//  return a.exec();
  return 0;
}
