#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    vcom(false)
{
    ui->setupUi(this);
    ui->progressBar->setValue (0);

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

    flashState = FLASH_STATE_IDLE;
    ui->statusBar->showMessage(tr("Started!"));

    //debug
    ui->lineEditFilepath->setText(tr("D:\\Installed Files\\qt\\firmwareUpdate\\ST-110_1.0.0_1.0.3V.aae"));
    //debug
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
    ui->groupBoxFirmware->setEnabled (connect);
}

void MainWindow::on_pushButtonBrowse_clicked()
{
    QString fileName;
    QString caption;
//    QString directory = QApplication::applicationDirPath();
    QString filter;

    if (ui->radioButtonSt->isChecked ()) {
        caption = tr("Open AAE File");
        filter = tr("AAE Files (*.aae)");
    } else if (ui->radioButtonPr->isChecked ()) {
        caption = tr("Open MOT File");
        filter = tr("MOT Files (*.mot)");
    } else {
        caption = tr("Open File");
        filter = tr("Files (*.*)");
    }

    fileName = QFileDialog::getOpenFileName(this, caption, "", filter);
    if (fileName != "") {
        ui->lineEditFilepath->setText (fileName);
    }
}
void MainWindow::on_pushButtonFlash_clicked()
{
    int answer;
//    quint32 size;

    m_File = ui->lineEditFilepath->text ();

    if (m_File.contains (tr("V.aae"), Qt::CaseSensitive)) {
        answer = QMessageBox::information( this, "Warning:: Wrong firmware",
                      "This firmware is normally used for STx10 USB version.\n"
                      "Do you still want to flash now?",
                      "Yes", "No",
                      0, 1 );
        if (answer == 1) {
            return;
        }
        vcom = true;
    } else {
        vcom = false;
    }

    hexFile.setFileName(m_File);
    if (hexFile.open(QIODevice::ReadOnly)) {
//        size = hexFile.size();
        m_Memory.clear();

        while (!hexFile.atEnd()) {
            QByteArray line = hexFile.readLine();
            m_Memory.append(line);
        }

        ui->listWidgetLog->clear();
        flashState = FLASH_STATE_GOTOBL;
        stReader->setUsbReader(vcom);
        hexFile.close();
    }
}
void MainWindow::onReaderPacketIn(const QByteArray &input)
{
    //qDebug() << "item: " << input;
    ui->listWidgetLog->addItem(QString(input));
    ui->listWidgetLog->scrollToBottom();
}

void MainWindow::on10msTimer()
{
    clk10msCounter++;

    if ((clk10msCounter % 50) == 0) {
        handleFlashState();
    }
}
void MainWindow::handleFlashState()
{
    switch (flashState) {
    case FLASH_STATE_IDLE:
        break;
    case FLASH_STATE_GOTOBL:
        if (stReader->gotoBootloaderMode()) {
            stReader->disconnectReader();
            connect(bootloader, SIGNAL(connection(bool)), this, SLOT(setConnectingControl(bool)));
            connect(bootloader, SIGNAL(raiseErrorMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
            connect(bootloader, SIGNAL(raiseStatusMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
            connect(bootloader, SIGNAL(nextpage(int)), this, SLOT(onFlashNextPage(int)));
            bootloader->connectReader(channel);
            bootloader->writeFirmware(m_Memory);
            bootloader->writeFirmwareStep();

            clk10msFlashNextpageTimeout = clk10msCounter + 500;
            ui->groupBoxFirmware->setEnabled(false);
            flashState = FLASH_STATE_FLASHING;
        }
        break;
    case FLASH_STATE_FLASHING:
        if (clk10msCounter > clk10msFlashNextpageTimeout) {
            flashState = FLASH_STATE_FAIL;
        }
        break;
    case FLASH_STATE_FLASHED:
        bootloader->disconnectReader();
        disconnect(bootloader, SIGNAL(connection(bool)), this, SLOT(setConnectingControl(bool)));
        disconnect(bootloader, SIGNAL(raiseErrorMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
        disconnect(bootloader, SIGNAL(raiseStatusMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
        disconnect(bootloader, SIGNAL(nextpage(int)), this, SLOT(onFlashNextPage(int)));
        stReader->connectReader();

        ui->groupBoxFirmware->setEnabled(false);
        flashState = FLASH_STATE_GOTOFW;
        break;
    case FLASH_STATE_GOTOFW:
        if (stReader->gotoFirmwareMode()) {
            flashState = FLASH_STATE_IDLE;
            //qDebug() << "-------- finished ----------";
        }
        break;
    case FLASH_STATE_FAIL:
        QMessageBox::information(this, "Flashing failed", "Please restart reader", "OK");

        bootloader->disconnectReader();
        disconnect(bootloader, SIGNAL(connection(bool)), this, SLOT(setConnectingControl(bool)));
        disconnect(bootloader, SIGNAL(raiseErrorMessage(QString)), ui->statusBar, SLOT(showMessage(QString)));
        disconnect(bootloader, SIGNAL(nextpage(int)), this, SLOT(onFlashNextPage(int)));

        flashState = FLASH_STATE_IDLE;
        //qDebug() << "-------- FLASH_STATE_FAIL ----------";
        break;
    default:
        flashState = FLASH_STATE_IDLE;
        break;
    }
}

void MainWindow::onFlashNextPage(int progressPercentage)
{
    ui->progressBar->setValue(progressPercentage);
    clk10msFlashNextpageTimeout = clk10msCounter + 500;
    if (bootloader->writeFirmwareStep()) {
        flashState = FLASH_STATE_FLASHED;
    }
}

void MainWindow::on_pushButtonTest_clicked()
{
//    stReader->sendTestData();
//    stReader->gotoBootloaderMode();
//    stReader->gotoFirmwareMode();
}
