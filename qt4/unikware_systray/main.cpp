#include "unikware.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Unikware w;
    w.show();

    qDebug() << "Run systray";

    return a.exec();
}
