#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>

class MyClass : public QObject
{
    Q_OBJECT
public:
    explicit MyClass(QObject *parent = 0);
    Q_INVOKABLE void cppMethod(const QString &msg);
    
signals:
    
public slots:
    void cppSlot(int number);
    
};

#endif // MYCLASS_H
