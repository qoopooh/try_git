#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include "prreader.h"
#include "streader.h"
#include "bootloader.h"

namespace Ui {
    class MainWindow;
}

// ----------------------------------------------------------------------------
// Set this to the maximum value of 63kB (for 'F340) because this needs to be
// known at compile-time. An array of this size is allocated as "m_Memory" in
#define MEMORY_SIZE 63*1024
#define APP_START_PAGE        0x09     // First application page (page 09)
#define PAGE_SIZE             0x200    // Pages are 512 bytes long
#define APP_START_ADDR        APP_START_PAGE*PAGE_SIZE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum FLASH_STATE{
        FLASH_STATE_IDLE,
        FLASH_STATE_GOTOBL,
        FLASH_STATE_GOTOBL_DELAY,
        FLASH_STATE_STARTBL,
        FLASH_STATE_FLASHING,
        FLASH_STATE_FLASHED,
        FLASH_STATE_GOTOFW,
        FLASH_STATE_FAIL,
        FLASH_STATE_UNDEF = 0xFE
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonRefresh_clicked();
    void on_pushButtonConnect_clicked();
    void on_pushButtonBrowse_clicked();
    void on_pushButtonFlash_clicked();
    void setConnectingControl(bool connect);
    void onReaderPacketIn(const QByteArray &input);
    void on10msTimer();
    void onFlashNextPage(int progressPercentage);

    void on_pushButtonTest_clicked();

private:
    Ui::MainWindow *ui;
    PrReader *prReader;
    StReader *stReader;
    BootLoader *bootloader;
    QString channel;
    bool vcom;

    QTimer *clk10msTimer;
    quint32 clk10msCounter;
    quint32 clk10msFlashNextpageTimeout;
    quint32 clk10msRebootDelay;
    /* Boot Loader */
    QString m_File;
    QFile hexFile;
    QByteArray m_Memory;
    FLASH_STATE flashState;
    /* Boot Loader */

    void getReaderChannels();
    /* Boot Loader */
    void handleFlashState();
    /* Boot Loader */
};

#endif // MAINWINDOW_H
