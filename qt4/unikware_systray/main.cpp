#include "unikware.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Unikware w;
    w.show();

    return a.exec();
}
