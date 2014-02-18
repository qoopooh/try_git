#ifndef UNIKWARE_H
#define UNIKWARE_H

#include <QSystemTrayIcon>
#include <QDialog>

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

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void setIcon(int index);
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void showMessage();
  void messageClicked();

private:
  void createIconGroupBox();
  void createMessageGroupBox();
  void createActions();
  void createTrayIcon();

  QGroupBox *iconGroupBox;
  QLabel *iconLabel;
  QComboBox *iconComboBox;
  QCheckBox *showIconCheckBox;

  QGroupBox *messageGroupBox;
  QLabel *typeLabel;
  QLabel *durationLabel;
  QLabel *durationWarningLabel;
  QLabel *titleLabel;
  QLabel *bodyLabel;
  QComboBox *typeComboBox;
  QSpinBox *durationSpinBox;
  QLineEdit *titleEdit;
  QTextEdit *bodyEdit;
  QPushButton *showMessageButton;

  QAction *minimizeAction;
  QAction *maximizeAction;
  QAction *restoreAction;
  QAction *quitAction;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;
};

#endif // UNIKWARE_H
