#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vcom(false)
{
    ui->setupUi(this);

    stReader = new StReader(this);
    bootloader = new BootLoader(this);
    clk10msTimer = new QTimer(this);
    getReaderChannels();

    connect(stReader, SIGNAL(connection(bool)), this, SLOT(setConnectingControl(bool)));
    connect(stReader, SIGNAL(raiseErrorMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
    connect(stReader, SIGNAL(raiseStatusMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
    connect(stReader, SIGNAL(dataReceived(QByteArray)), this, SLOT(onReaderPacketIn(QByteArray)));

    connect(clk10msTimer, SIGNAL(timeout()), this, SLOT(on10msTimer()));
    clk10msTimer->start(10);
    clk10msCounter = 0;

    ui->statusBar->showMessage(tr("Started!"));
}

MainWindow::~MainWindow()
{
    delete ui;
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
    //ui->comboBoxPort->setCurrentIndex(ui->comboBoxPort->count() - 1);
    ui->comboBoxPort->setCurrentIndex(0); // for ttyACM0
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
    //ui->groupBoxFirmware->setEnabled (connect);
}

void MainWindow::onReaderPacketIn(const QByteArray &input)
{
    ui->listWidgetLog->addItem(QString(input));
    ui->listWidgetLog->scrollToBottom();
}

void MainWindow::on10msTimer()
{
    clk10msCounter++;

    if ((clk10msCounter % 50) == 0) {
        //handleFlashState();
    }
}
