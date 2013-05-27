#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    listitem(NULL),
    vcom(false),
    prev_epc(""),
    prev_epc_count(0)
{
    ui->setupUi(this);
    createLogTable();

    stReader = new StReader(this);
    bootloader = new BootLoader(this);
    clk10msTimer = new QTimer(this);
    getReaderChannels();

    connect(stReader, SIGNAL(connection(bool)), this, SLOT(setConnectingControl(bool)));
    connect(stReader, SIGNAL(raiseErrorMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
    connect(stReader, SIGNAL(raiseStatusMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
    connect(stReader, SIGNAL(dataReceived(QByteArray)), this, SLOT(onReaderPacketIn(QByteArray)));
    connect(stReader, SIGNAL(readingEpc(QByteArray)), this, SLOT(onEpc(QByteArray)));
    connect(stReader, SIGNAL(readingEpcString(QString)), this, SLOT(onEpcString(QString)));

    connect(clk10msTimer, SIGNAL(timeout()), this, SLOT(on10msTimer()));
    clk10msTimer->start(10);
    clk10msCounter = 0;

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

  //ui->listWidgetLog->resize(w - 20, h - 150);
  ui->groupBoxControl->resize(w - 20, ui->groupBoxControl->size().height());
}

void MainWindow::createLogTable()
{
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
//    ui->comboBoxPort->setCurrentIndex(0); // for ttyACM0
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
  //ui->listWidgetLog->addItem(msg);
  //ui->listWidgetLog->scrollToBottom();
  ui->textEditLog->append(msg);
}

void MainWindow::onEpc(const QByteArray &ba)
{
  QString msg = QTime::currentTime().toString("hh:mm:ss.zzz") + " EPC: ";

  msg += ba.data();
  //listitem = new QListWidgetItem(msg);
  //ui->listWidgetLog->addItem(listitem);
  //ui->listWidgetLog->scrollToBottom();

  ui->textEditLog->append(msg);
}

void MainWindow::onEpcString(const QString &epc)
{
  insertDupplicatedTag(epc);
  //if (epc.compare(prev_epc) != 0) {
    //prev_epc = epc;
    //setShelfAndBag(epc);
  //}
}

void MainWindow::insertDupplicatedTag(const QString epc)
{
  QString msg;
  QString epc_id, epc_count;

  if (0) {
  //if ((epc.compare(prev_epc) == 0 && (ui->listWidgetLog->count()))) {
    ++prev_epc_count;

    msg = QTime::currentTime().toString("hh:mm:ss.zzz") + " EPC: ";
    msg += epc + ", " + QString::number(prev_epc_count);
    listitem->setText(msg);
  } else {
    prev_epc = epc;
    prev_epc_count = 1;
    msg = QTime::currentTime().toString("hh:mm:ss.zzz") + " EPC: ";
    msg += epc;
    setShelfAndBag(epc);
    msg += ", " + QString::number(prev_epc_count);
    listitem = new QListWidgetItem(msg);
    //ui->listWidgetLog->addItem(listitem);
  }

  //ui->listWidgetLog->scrollToBottom();
}

//void MainWindow::insertDupplicatedTag(const QString epc)
//{
  //QString msg;

  //if ((epc.compare(prev_epc) == 0 && (ui->listWidgetLog->count()))) {
    //++prev_epc_count;

    //msg = QString(listitem->text());
    //int comma_position = msg.lastIndexOf(",");
    //msg.remove(comma_position, msg.size() - comma_position);
    //msg += ", " + QString::number(prev_epc_count);
    //listitem->setText(msg);
  //} else {
    //prev_epc = epc;
    //prev_epc_count = 1;
    //msg = QTime::currentTime().toString("hh:mm:ss") + " EPC: ";
    //msg += epc;
    //setShelfAndBag(epc);
    //msg += ", " + QString::number(prev_epc_count);
    //listitem = new QListWidgetItem(msg);
    //ui->listWidgetLog->addItem(listitem);
  //}

  //ui->listWidgetLog->scrollToBottom();
//}

void MainWindow::setShelfAndBag(const QString epc)
{
  QString data = epc.mid(1);
  if ((epc[0] == 'S') && (data.compare(ui->lineEditShelf->text()) != 0)) {
    ui->lineEditShelf->setText(data);
    ui->lineEditShelf->setStyleSheet("QLineEdit{background: orange;}");
    shelf_changed_tout = 500;
  } else if ((epc[0] == 'B') && (data.compare(ui->lineEditBag->text()) != 0)) {
    ui->lineEditBag->setText(data);
    ui->lineEditBag->setStyleSheet("QLineEdit{background: orange;}");
    bag_changed_tout = 500;
  }
}

void MainWindow::on10msTimer()
{
    clk10msCounter++;

    if ((clk10msCounter % 50) == 0) {
    }

    if (shelf_changed_tout) {
      --shelf_changed_tout;
      if (!shelf_changed_tout) {
        ui->lineEditShelf->setStyleSheet("QLineEdit{background: white;}");
      }
    }
    if (bag_changed_tout) {
      --bag_changed_tout;
      if (!bag_changed_tout) {
        ui->lineEditBag->setStyleSheet("QLineEdit{background: white;}");
      }
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
  //stReader->sendTestData();
}

void MainWindow::on_pushButtonClear_clicked()
{
  //ui->listWidgetLog->clear();
  ui->textEditLog->clear();
  prev_epc = "";
  prev_epc_count = 0;
}

