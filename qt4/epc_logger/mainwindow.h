#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QResizeEvent>
#include <QListWidgetItem>
#include "streader.h"
#include "aaecommand.h"
#include "epctreemodel.h"
#include "epcdb.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);
    void createLogTable();

private slots:
    void on_pushButtonRefresh_clicked();
    void setConnectingControl(bool connect);
    void onReaderPacketIn(const QByteArray &input);
    void onEpc(const QByteArray &);
    void onEpcString(const QString &);
    void onAttenuation(const int &);
    void on10msTimer();
    void onExportDatabase();
    void onDeleteDatabase();

    void on_checkBoxConnect_clicked(bool checked);
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonSingle_clicked();
    void on_pushButtonClear_clicked();

private:
    Ui::MainWindow *ui;
    EpcTreeModel *model;
    StReader *stReader;
    QString channel;
    bool vcom;
    int count_changed_tout;
    int db_changed_tout;
    int m_attn;
    EpcDb *m_db;

    QTimer *clk10msTimer;

    void getReaderChannels();
    void setEpcNumber(const QByteArray &);
    void updateActions();
};

#endif // MAINWINDOW_H
