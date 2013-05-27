#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QResizeEvent>
#include "streader.h"
#include "aaecommand.h"
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

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButtonRefresh_clicked();
    void setConnectingControl(bool connect);
    void onReaderPacketIn(const QByteArray &input);
    void onEpc(const QByteArray &);
    void onEpcString(const QString &);
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
    QString prev_epc;
    int prev_epc_count;
    int shelf_changed_tout;
    int bag_changed_tout;

    QTimer *clk10msTimer;
    quint32 clk10msCounter;

    void getReaderChannels();
    void insertDupplicatedTag(const QString);
    void setShelfAndBag(const QString);
};

#endif // MAINWINDOW_H
