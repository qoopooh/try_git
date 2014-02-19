#ifndef UNIKPROCESS_H
#define UNIKPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class UnikProcess : public QObject
{
  Q_OBJECT
public:
  explicit UnikProcess(QObject *parent = 0);

  bool isRunning(QString proc);
  bool exec(QString proc);

signals:

public slots:

};

#endif // UNIKPROCESS_H
