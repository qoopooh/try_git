#ifndef UNIKWARE_H
#define UNIKWARE_H

#include <QtGui>
#include <QSystemTrayIcon>
#include <QDialog>
#include <QTimer>
#include <QProcess>
#include <QtNetwork/QHostInfo>

#include "database.h"

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

class Unikware : public QDialog
{
  Q_OBJECT

public:
  Unikware(QWidget *parent = 0);
  ~Unikware();

  void setVisible(bool visible);

  enum TimerState { Idle, Running, LastMinute };

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void onTimeout();
  void onTimeExtend();

private:
  void createNotifyGroupBox();
  void createActions();
  void createTrayIcon();
  void showMessage(int);
  bool isProcessRunning();
  bool killProcess();
  void setMinuteLeft(int);
  int calculateRemainMinute();

  QIcon m_icon;
  QGroupBox *notifyGroupBox;
  QLabel *timeLabel;
  QLineEdit *timeLineEdit;
  QPushButton *timeExtendPushButton;

  QAction *minimizeAction;
  QAction *maximizeAction;
  QAction *restoreAction;
  QAction *quitAction;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  bool f_hide;
  bool f_first_close;
  int m_count;
  TimerState m_state;
  QTimer *m_timer;
  Database *m_db;
  QString client;
};

#endif // UNIKWARE_H
