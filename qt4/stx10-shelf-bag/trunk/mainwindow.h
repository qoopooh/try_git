#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include "streader.h"
#include "bootloader.h"

namespace Ui {
    class MainWindow;
}

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
    void setConnectingControl(bool connect);
    void onReaderPacketIn(const QByteArray &input);
    void on10msTimer();

    void on_checkBoxConnect_clicked(bool checked);

    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();

    void on_pushButtonSingle_clicked();

    void on_pushButtonClear_clicked();

private:
    Ui::MainWindow *ui;
    StReader *stReader;
    BootLoader *bootloader;
    QString channel;
    bool vcom;

    QTimer *clk10msTimer;
    quint32 clk10msCounter;

    void getReaderChannels();
};

#endif // MAINWINDOW_H
