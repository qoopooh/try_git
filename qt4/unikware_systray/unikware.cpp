#include "unikware.h"

const QString PROCESS("TCPIP-232-V3.2.exe");
const QString CLIENT("berm");
const int TIME_INTERVAL = 2000;
const int MINUTE = (8 * 1000) / TIME_INTERVAL;
const int WAIT_COUNT = 2 * MINUTE;
const int LAST_MINUTE = MINUTE;

Unikware::Unikware(QWidget *parent)
  : QDialog(parent), f_startup(true), f_first_close(true), m_state(Idle),
    client(CLIENT)
{
  createNotifyGroupBox();

  createActions();
  createTrayIcon();

  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()),
          this, SLOT(onTimeout()));
  connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(show()));
//  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
//          this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(notifyGroupBox);
  setLayout(mainLayout);

  m_icon.addPixmap(QPixmap(QString::fromUtf8(":/images/unik")),
  QIcon::Normal, QIcon::Off);
  trayIcon->setIcon(m_icon);
  trayIcon->show();

  setWindowIcon(m_icon);
  setWindowTitle(tr("Unikware Monitor"));
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
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
      break;
    case QSystemTrayIcon::MiddleClick:
      showMessage();
      break;
    default:
      ;
  }
}

void Unikware::showMessage()
{
  QString msg = PROCESS + " will be closed in 1 minute";
  trayIcon->showMessage(this->windowTitle(), msg,
                        QSystemTrayIcon::Information, 45000);
}

void Unikware::onTimeout()
{
  qDebug() << "state" << m_state
           << "count" << m_count;
  if (!isProcessRunning()) {
    if (m_state != Idle) {
      m_db->logout(client);
    }
    m_state = Idle;
    if (f_startup) {
      f_startup = false;
      this->hide();
    }
    return;
  }

  if (m_state == Idle) {
    m_db->login(client);
    m_state = Running;
    m_count = WAIT_COUNT;
    return;
  }

  if (m_count) {
    --m_count;
    if ((m_count % MINUTE) == 0) {
      int min = (m_count / MINUTE);
      QString msg("%1");

      if (m_state == Running) {
        ++min;
      }
      timeLineEdit->setText(msg.arg(min));
    }
  }

  if (m_count == 0) {
    if (m_state == Running) {
      m_state = LastMinute;
      m_count = LAST_MINUTE;
      showMessage();
    } else if (m_state == LastMinute) {
      killProcess();
    }
  }
}

bool Unikware::isProcessRunning()
{
  QProcess tasklist;
  tasklist.start(
        "tasklist",
        QStringList() << "/NH"
                      << "/FO" << "CSV"
                      << "/FI" << QString("IMAGENAME eq %1").arg(PROCESS));
  tasklist.waitForFinished();
  QString output = tasklist.readAllStandardOutput();
  return output.startsWith(QString("\"%1").arg(PROCESS));
}

bool Unikware::killProcess()
{
  QProcess taskkill;
  taskkill.start(
        "taskkill",
        QStringList() << "/F"
                      << "/IM" << PROCESS);
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

  QHBoxLayout *timeLayout = new QHBoxLayout;
  timeLayout->addWidget(timeLabel);
  timeLayout->addWidget(timeLineEdit);
  notifyGroupBox->setLayout(timeLayout);
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

