#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString k_version("V1.00");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vcom(false),
    attenuation_tout(200),
    m_attn(-1),
    m_db(new EpcDb())
{
  ui->setupUi(this);
  createLogTable();

  stReader = new StReader(this);
  clk10msTimer = new QTimer(this);
  getReaderChannels();

  connect(stReader, SIGNAL(connection(bool)), this, SLOT(setConnectingControl(bool)));
  connect(stReader, SIGNAL(raiseErrorMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
  connect(stReader, SIGNAL(raiseStatusMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
  connect(stReader, SIGNAL(dataReceived(QByteArray)), this, SLOT(onReaderPacketIn(QByteArray)));
  connect(stReader, SIGNAL(readingEpc(QByteArray)), this, SLOT(onEpc(QByteArray)));
  connect(stReader, SIGNAL(attenuation(int)), this, SLOT(onAttenuation(int)));
  connect(ui->action_Info, SIGNAL(triggered()), this, SLOT(onInfo()));
  connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionE_xport, SIGNAL(triggered()), this, SLOT(onExportDatabase()));
  connect(ui->action_Delete, SIGNAL(triggered()), this, SLOT(onDeleteDatabase()));

  connect(clk10msTimer, SIGNAL(timeout()), this, SLOT(on10msTimer()));
  clk10msTimer->start(10);

  channel = ui->comboBoxPort->currentText();
  stReader->connectReader(channel);
  if (ui->comboBoxPort->count() > 0)
    ui->checkBoxConnect->setChecked(true);
  ui->statusBar->showMessage(tr("Started!"));
  this->setWindowTitle(this->windowTitle() + k_version);
}

MainWindow::~MainWindow()
{
  delete ui;
  m_db->close();
  delete m_db;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  int w = event->size().width();
  int h = event->size().height();

  ui->tabWidgetLog->resize(w - 20, h - 170);
  ui->treeViewLog->resize(w - 20, h - 190);
  ui->textEditLog->resize(w - 20, h - 190);
  ui->groupBoxControl->resize(w - 20, ui->groupBoxControl->size().height());
}

void MainWindow::createLogTable()
{
  model = new EpcTreeModel("");
  ui->treeViewLog->setModel(model);
  ui->treeViewLog->setColumnWidth(0, 200);
  ui->treeViewLog->setWindowTitle(QObject::tr("EPC Reading"));

  delRowAction = new QAction(tr("&Delete EPC"), this);
  delRowAction->setIcon(QIcon(":/images/cancel.png"));
  delRowAction->setShortcut(tr("Ctrl+D"));
  delRowAction->setStatusTip(tr("Delete test record from database"));
  connect(delRowAction, SIGNAL(triggered()), this, SLOT(onDeleteEpc()));
  ui->treeViewLog->addAction(delRowAction);
  ui->treeViewLog->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::on_pushButtonRefresh_clicked()
{
  getReaderChannels();
}
void MainWindow::getReaderChannels()
{
  QList<QString> channels = AaeReader::discovery();

  ui->comboBoxPort->clear();
  for (int i = 0; i<channels.size(); i++){
    ui->comboBoxPort->addItem(channels.at(i));
  }
  ui->comboBoxPort->setCurrentIndex(ui->comboBoxPort->count() - 1); // for window
  ui->checkBoxConnect->setEnabled(ui->comboBoxPort->count() > 0);
}

void MainWindow::on_checkBoxConnect_clicked(bool checked)
{
  if (checked) {
    if (stReader->isConnected())
      return;
    channel = ui->comboBoxPort->currentText();
    stReader->connectReader(channel);
  } else {
    if (!stReader->isConnected())
      return;
    stReader->disconnectReader();
  }
}

void MainWindow::setConnectingControl(bool connect)
{
  ui->groupBoxConnection->setEnabled (!connect);
  ui->groupBoxControl->setEnabled (connect);
}

void MainWindow::onReaderPacketIn(const QByteArray &input)
{
  QString msg = QTime::currentTime().toString("hh:mm:ss.zzz") + " ";
  AaeCommand::AAE_COMMAND cmd = stReader->getCommandName();

  switch (cmd) {
    case AaeCommand::CmdGetSoftwareRevision:
      msg += "software ver.: ";
      break;
    case AaeCommand::CmdGetSerialNumber:
      msg += "sn.: ";
      break;
    case AaeCommand::CmdGetAttenuation:
      msg += "Attenuation.: ";
      break;
    case AaeCommand::CmdInventoryCyclic:
      msg += "Inventory cyclic: ";
      break;
    case AaeCommand::CmdInventorySingle:
      msg += "Inventory single: ";
      break;
    default:
      msg = "";
      break;
  }

  if (msg.length() < 1)
    return;
  msg += QString(input);
  ui->textEditLog->append(msg);
}

void MainWindow::onEpc(const QByteArray &ba)
{
  QString msg = QTime::currentTime().toString("hh:mm:ss.zzz") + " EPC: ";

  msg += ba.data();
  ui->textEditLog->append(msg);
  setEpcNumber(ba.data());
}

void MainWindow::onEpcString(const QString &epc)
{
  model->insertEpc(epc);
  updateActions();
}

void MainWindow::onAttenuation(const int &attn)
{
  m_attn = attn;
}

void MainWindow::setEpcNumber(const QByteArray &epchex)
{
  static int tree_count = 0;
  static int db_count = 0;
  onEpcString(epchex);
  if (model->count() != tree_count) {
    tree_count = model->count();
    ui->lineEditCount->setText(QString::number(tree_count));
    ui->lineEditCount->setStyleSheet("QLineEdit{background: orange;}");
    count_changed_tout = 300;
    if (m_db->addEpc(epchex, m_attn)) {
      if (db_count == m_db->getEpcCount())
        return;
      db_count = m_db->getEpcCount();
      ui->lineEditTotal->setText(QString::number(m_db->getEpcCount()));
      ui->lineEditTotal->setStyleSheet("QLineEdit{background: orange;}");
      db_changed_tout = 300;
    } else {
      disconnect(stReader, SIGNAL(readingEpc(QByteArray)), this, SLOT(onEpc(QByteArray)));
      QMessageBox::critical(0, qApp->tr("Cannot open database"),
                            m_db->error(), QMessageBox::Cancel);
      QCoreApplication::exit(-1);
    }
  }
}

void MainWindow::updateActions()
{
  ui->treeViewLog->reset();
}

void MainWindow::on10msTimer()
{
  if (count_changed_tout) {
    --count_changed_tout;
    if (!count_changed_tout) {
      ui->lineEditCount->setStyleSheet("QLineEdit{background: white;}");
    }
  }
  if (db_changed_tout) {
    --db_changed_tout;
    if (!db_changed_tout) {
      ui->lineEditTotal->setStyleSheet("QLineEdit{background: white;}");
    }
  }
  if (attenuation_tout) {
    --attenuation_tout;
    if (!attenuation_tout) {
      stReader->getAttenuation();
    }
  }
}

void MainWindow::onExportDatabase()
{
  QString fn = QFileDialog::getSaveFileName(this, tr("Save Database as..."),
                                            QString(), tr("CSV files (*.csv)"));
  if (fn.isEmpty())
    return;
  if (!fn.endsWith(".csv", Qt::CaseInsensitive))
    fn += ".csv"; // default
  m_db->report(fn);
}

void MainWindow::onDeleteDatabase()
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Database Clean Up",
                                "Do you really want to delete database?",
                                QMessageBox::Yes|QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    qDebug() << "Yes was clicked";
    m_db->clear();
  } else {
    qDebug() << "Yes was *not* clicked";
  }
}

void MainWindow::onDeleteEpc()
{
  QMessageBox::StandardButton reply;

  QModelIndex index = ui->treeViewLog->currentIndex();
  QString var = index.data().toString();
  reply = QMessageBox::question(this, "Delete EPC record",
                                QString("Do you really want to delete %1?").arg(var),
                                QMessageBox::Yes|QMessageBox::No);
  if (reply == QMessageBox::Yes) {
//    m_db->clear();
    m_db->deleteEpc(var);
  }
}

void MainWindow::onInfo()
{
  QString paths;
  foreach(QString str, QApplication::libraryPaths()) {
    paths += str + ":";
  }

  QMessageBox::information(this, tr("Parameter"), paths);
}

void MainWindow::on_pushButtonStart_clicked()
{
  stReader->inventoryCyclic(true);
  ui->tabWidgetLog->setCurrentIndex(0);
}

void MainWindow::on_pushButtonStop_clicked()
{
  stReader->inventoryCyclic(false);
}

void MainWindow::on_pushButtonAttenuation_clicked()
{
  stReader->getAttenuation();
  ui->tabWidgetLog->setCurrentIndex(1);
}

void MainWindow::on_pushButtonClear_clicked()
{
  ui->lineEditCount->setText("-");
  ui->lineEditCount->setStyleSheet("QLineEdit{background: white;}");
  ui->lineEditTotal->setText(QString::number(m_db->getEpcCount()));
  ui->lineEditTotal->setStyleSheet("QLineEdit{background: white;}");
  ui->textEditLog->clear();
  delete model;
  model = new EpcTreeModel("");
  ui->treeViewLog->setModel(model);
  updateActions();
}

