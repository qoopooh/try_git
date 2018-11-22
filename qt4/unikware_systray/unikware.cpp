#include "unikware.h"

const QString APP("Unikware Monitor V0.10");
const QString UNIK_PROC("unikware.exe");
const QString URL("http://192.168.1.44/unikmon/");
const int TIME_INTERVAL = 2000;                     // milliseconds
const int MIN_UNIT = (60 * 1000) / TIME_INTERVAL;   // -> 30
const int TIMEOUT_MINUTES = 30;
//const int TIMEOUT_MINUTES = 3;
const int WAIT_COUNT = TIMEOUT_MINUTES * MIN_UNIT + 2;
const int LAST_MINUTE = 2 * MIN_UNIT;

Unikware::Unikware(QWidget *parent) : QDialog(parent),
    f_first_close(true),
    m_count(WAIT_COUNT), m_state(Idle)
{
  createNotifyGroupBox();
  createInfoGroupBox();

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
  connect(linkPushButton, SIGNAL(clicked()), this, SLOT(openLink()));

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(notifyGroupBox);
  mainLayout->addWidget(infoGroupBox);
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

  timeExtendPushButton->setDisabled(true);
  calculateRemainMinute();
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
    event->ignore();
    hide();
  }
}

void Unikware::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
      show();
      break;
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

/**
 * Execute every 2 seconds
 */
void Unikware::onTimeout()
{
  bool app_running = isProcessRunning(); // check process
  qDebug() << "state" << m_state
           << "count" << m_count
           << "running" << app_running;

  int min = calculateRemainMinute();
  switch (m_state) {
    case Idle:

      if (!app_running)
        return;

      m_db->login(client);
      m_state = Hiding;
      break;

    case Hiding:
      m_state = Running;
      m_count = WAIT_COUNT;
      hide();
      break;

    case Running:
      if (!app_running) {
        m_db->logout(client);
        exit(0);
      }

      if (m_count < LAST_MINUTE) {
        m_state = LastMinute;

        show();
        showMessage(min);
        timeExtendPushButton->setDisabled(false);
      }
      break;

    case LastMinute:
      if (m_count < 2) {
        m_state = Kill;
      }
      break;

    case Kill:
      killProcess();
      m_db->logout(client);
      exit(0);
      break;

    default:
      break;
  }

  --m_count;
} // onTimeout

/**
 * User clicked extending time
 */
void Unikware::onTimeExtend()
{
  m_state = Running;
  m_count = WAIT_COUNT;
  timeExtendPushButton->setDisabled(true);
  hide();
}

/**
 * Calculate minute left and show balloon
 */
int Unikware::calculateRemainMinute()
{
  QString minutes("-");
  int min = (m_count / MIN_UNIT);

  if (min >= 0) {
    minutes = QString::number(min);
  }
  timeLineEdit->setText(minutes);
  trayIcon->setToolTip(minutes + " minute(s)");

  return min;
}

/**
 * Open external link on browser
 */
void Unikware::openLink()
{
  qDebug() << "openLink";
  QDesktopServices::openUrl(QUrl(URL));
}

/**
 * Check unikware running
 *
 * @return true if it's running
 */
bool Unikware::isProcessRunning()
{
  QProcess tasklist;
  QStringList args(QStringList() << "/NH"
                  << "/FO" << "CSV"
                  << "/FI" << QString("IMAGENAME eq %1").arg(UNIK_PROC));
  tasklist.start("tasklist", args);
  tasklist.waitForFinished();
  QString output = tasklist.readAllStandardOutput().trimmed().toLower();

  return output.startsWith(QString("\"%1").arg(UNIK_PROC));
  //return true; // TODO: removed
}

/**
 * Close program
 */
bool Unikware::killProcess()
{
  QProcess taskkill;
  taskkill.start(
        "taskkill",
        QStringList() << "/F"
                      << "/IM" << UNIK_PROC);
  taskkill.waitForFinished();
  QString output = taskkill.readAllStandardOutput();
  qDebug() << "Killed" << UNIK_PROC << ":" << output;
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

void Unikware::createInfoGroupBox()
{
  infoGroupBox = new QGroupBox(tr("Usage Information"));
  linkPushButton = new QPushButton(URL);
  QHBoxLayout *infoLayout = new QHBoxLayout;
  infoLayout->addWidget(linkPushButton);
  infoGroupBox->setLayout(infoLayout);
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
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
}
