#ifndef UNIKPROCESS_H
#define UNIKPROCESS_H

#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDebug>

class UnikProcess : public QObject
{
  Q_OBJECT
public:
  explicit UnikProcess(QObject *parent = 0);

  bool isRunning(QString proc);

  /**
   * Run program
   *
   * @param proc process name
   * @param path of target program
   */
  bool exec(QString proc, QString path="");

signals:

public slots:

};

#endif // UNIKPROCESS_H
