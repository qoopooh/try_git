#include "unikware.h"

const QString APP("Unikware Monitor V0.3");
const QString UNIK_PROC("unikware.exe");
const QString CLIENT("berm");
//const int TIME_INTERVAL = 3000;
//const int MIN_UNIT = (8 * 1000) / TIME_INTERVAL;
const int TIME_INTERVAL = 10000;
const int MIN_UNIT = (60 * 1000) / TIME_INTERVAL;
const int WAIT_COUNT = 19 * MIN_UNIT;
const int LAST_MINUTE = 1 * MIN_UNIT;

Unikware::Unikware(QWidget *parent)
  : QDialog(parent), f_hide(true), f_first_close(true), m_state(Idle),
    client(CLIENT)
{
  createNotifyGroupBox();

  createActions();
  createTrayIcon();

  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()),
          this, SLOT(onTimeout()));
  connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(show()));
  connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(show()));
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
  connect(timeExtendPushButton, SIGNAL(clicked()), this, SLOT(onTimeExtend()));

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(notifyGroupBox);
  setLayout(mainLayout);

  m_icon.addPixmap(QPixmap(QString::fromUtf8(":/images/unik")),
  QIcon::Normal, QIcon::Off);
  trayIcon->setIcon(m_icon);
  trayIcon->show();

  setWindowIcon(m_icon);
  setWindowTitle(APP);
  m_timer->start(TIME_INTERVAL);
  m_db = new Database(this);
  QHostInfo hostInfo;
  hostInfo = QHostInfo::fromName(QHostInfo::localHostName());
  client = QHostInfo::localHostName();
}

Unikware::~Unikware()
{

}

void Unikware::setVisible(bool visible)
{
  minimizeAction->setEnabled(visible);
  maximizeAction->setEnabled(!isMaximized());
  restoreAction->setEnabled(isMaximized() || !visible);
  QDialog::setVisible(visible);
}

void Unikware::closeEvent(QCloseEvent *event)
{
  if (trayIcon->isVisible()) {
    if (f_first_close) {
      f_first_close = false;
      QMessageBox::information(this, tr("Systray"),
          tr("The program will keep running in the "
          "system tray. To terminate the program, "
          "choose <b>Quit</b> in the context menu "
          "of the system tray entry."));
    }
    hide();
    event->ignore();
  }
}

void Unikware::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
  case QSystemTrayIcon::DoubleClick:
  case QSystemTrayIcon::MiddleClick:
    show();
  case QSystemTrayIcon::Trigger:
  default:
    break;
  }
}

void Unikware::showMessage(int min)
{
  QString msg = UNIK_PROC + " will be closed in %1 minute (s)";
  trayIcon->showMessage(this->windowTitle(),
                        msg.arg(min),
                        QSystemTrayIcon::Information, 45000);
}

void Unikware::onTimeout()
{
  qDebug() << "state" << m_state
           << "count" << m_count;

  if (f_hide) {
    f_hide = false;
    this->hide();
  }

  if (!isProcessRunning()) {
    if (m_state != Idle) {
      m_db->logout(client);
//      setMinuteLeft(-1);
      exit(0);
    }
    m_state = Idle;
    return;
  }

  if (m_state == Idle) {
    m_db->login(client);
    m_state = Running;
    m_count = WAIT_COUNT;
    timeExtendPushButton->setDisabled(true);
    return;
  }

  int min = 0;
  if (m_count) {
    --m_count;
    if ((m_count % MIN_UNIT) == 0) {
      min = calculateRemainMinute();
    }
  }

  if (m_count == 0) {
    if (m_state == Running) {
      m_state = LastMinute;
      m_count = LAST_MINUTE;
      showMessage(min);
      show();
      timeExtendPushButton->setDisabled(false);
    } else if (m_state == LastMinute) {
      killProcess();
    }
  }
}

void Unikware::onTimeExtend()
{
  f_hide = true;
  m_count = WAIT_COUNT;
  calculateRemainMinute();
  m_state = Running;
  timeExtendPushButton->setDisabled(true);
}

bool Unikware::isProcessRunning()
{
  QProcess tasklist;
  QStringList args(QStringList() << "/NH"
                  << "/FO" << "CSV"
                  << "/FI" << QString("IMAGENAME eq %1").arg(UNIK_PROC));
  tasklist.start("tasklist", args);
  tasklist.waitForFinished();
  QString output = tasklist.readAllStandardOutput().trimmed().toLower();
//  qDebug() << args << output;
  return output.startsWith(QString("\"%1").arg(UNIK_PROC));
}

bool Unikware::killProcess()
{
  QProcess taskkill;
  taskkill.start(
        "taskkill",
        QStringList() << "/F"
                      << "/IM" << UNIK_PROC);
  taskkill.waitForFinished();
  QString output = taskkill.readAllStandardOutput();
  qDebug() << output;
  return output.startsWith("SUCCESS");
}

void Unikware::createNotifyGroupBox()
{
  notifyGroupBox = new QGroupBox(tr("Time remaining"));
  timeLabel = new QLabel("Minute(s):");
  timeLineEdit = new QLineEdit("-");
  timeLineEdit->setReadOnly(true);
  timeLineEdit->setAlignment(Qt::AlignHCenter);
  timeExtendPushButton = new QPushButton(tr("Extend the Time"));

  QHBoxLayout *timeLayout = new QHBoxLayout;
  timeLayout->addWidget(timeLabel);
  timeLayout->addWidget(timeLineEdit);
  timeLayout->addWidget(timeExtendPushButton);
  notifyGroupBox->setLayout(timeLayout);
}

void Unikware::setMinuteLeft(int m)
{
  QString min("-");

  if (m >= 0) {
    min = QString::number(m);
  }
  timeLineEdit->setText(min);
  trayIcon->setToolTip(min + " minute(s)");
}

int Unikware::calculateRemainMinute()
{
  int min = (m_count / MIN_UNIT);
  if (m_state == Running) {
    min += (LAST_MINUTE / MIN_UNIT);
  }
  setMinuteLeft(min);

  return min;
}

void Unikware::createActions()
{
  minimizeAction = new QAction(tr("Mi&nimize"), this);
  connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

  maximizeAction = new QAction(tr("Ma&ximize"), this);
  connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

  restoreAction = new QAction(tr("&Restore"), this);
  connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

  quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Unikware::createTrayIcon()
{
  trayIconMenu = new QMenu(this);
//  trayIconMenu->addAction(minimizeAction);
//  trayIconMenu->addAction(maximizeAction);
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
}
