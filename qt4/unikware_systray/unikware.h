#ifndef UNIKWARE_H
#define UNIKWARE_H

#include <QtGui>
#include <QSystemTrayIcon>
#include <QDialog>
#include <QTimer>
#include <QProcess>
#include <QtNetwork/QHostInfo>
#include <QDesktopServices>

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

  enum TimerState {
      Idle,
      Hiding,
      Running,
      LastMinute,
      Kill
  };

protected:

  void closeEvent(QCloseEvent *event);

private slots:

  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void onTimeout();

  /**
   * User clicked extending time
   */
  void onTimeExtend();

  /**
   * Open external link on browser
   */
  void openLink();

private:
  void createNotifyGroupBox();
  void createInfoGroupBox();
  void createActions();
  void createTrayIcon();
  void showMessage(int);

  /**
   * Check unikware running
   *
   * @return true if it's running
   */
  bool isProcessRunning();

  /**
   * Close program
   */
  bool killProcess();

  /**
   * Calculate minute left and show balloon
   */
  int calculateRemainMinute();

  QIcon m_icon;
  QGroupBox *notifyGroupBox;
  QLabel *timeLabel;
  QLineEdit *timeLineEdit;
  QPushButton *timeExtendPushButton;

  QGroupBox *infoGroupBox;
  QPushButton *linkPushButton;

  QAction *minimizeAction;
  QAction *maximizeAction;
  QAction *restoreAction;
  QAction *quitAction;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  bool f_first_close;
  int m_count;
  TimerState m_state;
  QTimer *m_timer;
  Database *m_db;
  QString client;
};

#endif // UNIKWARE_H
