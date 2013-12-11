#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_checkBoxConnect_toggled(bool checked);

  void onConnect();
  void onRead();

private:
  Ui::MainWindow *ui;
  QTcpSocket *tcpSocket;


  void log(QString str);
};

#endif // MAINWINDOW_H
