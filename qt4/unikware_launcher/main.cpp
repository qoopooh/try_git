#include "unikprocess.h"

const QString UNIK_PROC("unikware.exe");
const QString UNIKMON_PROC("unikware_systray-v0.4.exe");
const QString PATH("N:\\Program");
const QString UNIKWARE = PATH + "\\" + UNIK_PROC;
const QString UNIKWARE_MON = PATH + "\\UnikwareMonitor\\" + UNIKMON_PROC;

int main(int argc, char *argv[])
{
  qDebug() << argv[0] << argc;
  UnikProcess process;
  if (!process.isRunning(UNIKMON_PROC))
    process.exec(UNIKWARE_MON);
  if (!process.isRunning(UNIK_PROC))
    process.exec(UNIKWARE, PATH);
  return 0;
}
