#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vcom(false),
    prev_epc(""),
    prev_epc_count(0),
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
  connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionE_xport, SIGNAL(triggered()), this, SLOT(onExportDatabase()));
  connect(ui->action_Delete, SIGNAL(triggered()), this, SLOT(onDeleteDatabase()));

  connect(clk10msTimer, SIGNAL(timeout()), this, SLOT(on10msTimer()));
  clk10msTimer->start(10);

  channel = ui->comboBoxPort->currentText();
  stReader->connectReader(channel);
  ui->checkBoxConnect->setChecked(true);
  ui->statusBar->showMessage(tr("Started!"));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  int w = event->size().width();
  int h = event->size().height();

  ui->tabWidgetLog->resize(w - 20, h - 150);
  ui->treeViewLog->resize(w - 20, h - 170);
  ui->textEditLog->resize(w - 20, h - 170);
  ui->groupBoxControl->resize(w - 20, ui->groupBoxControl->size().height());
}

void MainWindow::createLogTable()
{
  model = new EpcTreeModel("");
  ui->treeViewLog->setModel(model);
  ui->treeViewLog->setColumnWidth(0, 200);
  ui->treeViewLog->setWindowTitle(QObject::tr("EPC Reading"));
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

void MainWindow::setEpcNumber(const QByteArray &epchex)
{
  static int tree_count = 0;

  onEpcString(epchex);
  if (model->count() != tree_count) {
    tree_count = model->count();
    ui->lineEditCount->setText(QString::number(tree_count));
    ui->lineEditCount->setStyleSheet("QLineEdit{background: orange;}");
    count_changed_tout = 300;
    if (m_db->addEpc(epchex, 0)) {
      ui->lineEditTotal->setText(QString::number(m_db->getEpcCount()));
      ui->lineEditTotal->setStyleSheet("QLineEdit{background: orange;}");
      db_changed_tout = 300;
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
}

void MainWindow::onExportDatabase()
{
  QString fn = QFileDialog::getSaveFileName(this, tr("Save Database as..."),
                                            QString(), tr("CSV files (*.csv)"));
  if (fn.isEmpty())
    return;
  if (!fn.endsWith(".csv", Qt::CaseInsensitive))
    fn += ".csv"; // default
  qDebug() << "File name: " << fn;
}

void MainWindow::onDeleteDatabase()
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Database Clean Up", "Do you really want to delete database?",
                                QMessageBox::Yes|QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    qDebug() << "Yes was clicked";
    close();
  } else {
    qDebug() << "Yes was *not* clicked";
  }
}

void MainWindow::on_pushButtonStart_clicked()
{
  stReader->inventoryCyclic(true);
}

void MainWindow::on_pushButtonStop_clicked()
{
  stReader->inventoryCyclic(false);
}

void MainWindow::on_pushButtonSingle_clicked()
{
  stReader->inventorySingle();
}

void MainWindow::on_pushButtonClear_clicked()
{
  prev_epc = "";
  prev_epc_count = 0;

  ui->lineEditCount->setText("-");
  ui->lineEditCount->setStyleSheet("QLineEdit{background: white;}");
  ui->lineEditTotal->setText("-");
  ui->lineEditTotal->setStyleSheet("QLineEdit{background: white;}");
  ui->textEditLog->clear();
  delete model;
  model = new EpcTreeModel("");
  ui->treeViewLog->setModel(model);
  updateActions();
}
