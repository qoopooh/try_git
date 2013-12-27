#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_checkBoxConnect_toggled(bool checked)
{
    if (checked) {
        ui->lineEditIp->setText("Check");
    } else {
        ui->lineEditIp->setText("Uncheck");
    }
}

void Dialog::on_pushButtonVersion_clicked()
{
    ui->lineEditIp->setText("Button");
}

void Dialog::on_pushButtonClear_clicked()
{
    ui->lineEditIp->setText("");
}
