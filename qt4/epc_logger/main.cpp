#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QStringList str(a.libraryPaths());
//  QStringList str;
//  str.append(".");
  str.append("./sqldrivers/");
  a.setLibraryPaths(str);
//  a.setLibraryPaths(str.mid(1));
  qDebug()<<"my library path : "<<a.libraryPaths();
//  QLibrary sqlib("qsqlite4");
//  sqlib.load();
//  qDebug()<<"my library loaded"<<sqlib.isLoaded();

  MainWindow w;
  w.show();

  return a.exec();
}
