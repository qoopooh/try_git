#include "myclass.h"

MyClass::MyClass(QObject *parent) :
  QObject(parent)
{
}

void MyClass::cppSlot(const QString &msg)
{
  qDebug() << "Called the C++ slot with message:" << msg;
}

