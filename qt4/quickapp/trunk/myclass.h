#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>
#include <QDebug>

class MyClass : public QObject
{
  Q_OBJECT
public:
    MyClass(QObject *parent = 0);
public slots:
  void cppSlot(const QString &msg);
};

#endif /* MYCLASS_H */

