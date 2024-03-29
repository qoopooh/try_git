﻿#include <QtWidgets>
#include "register.h"

Register::Register(QWidget *parent) :
  QDialog(parent)
{
  QCoreApplication::setOrganizationName("A-touch");
  QCoreApplication::setApplicationName(APP);

  createVerticalGroupBox();
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(verticalGroupBox);
  setLayout(mainLayout);
  setWindowTitle(APP_TITLE + " " + VERSION);
  setWindowIcon(QIcon(":/images/blue.png"));

  connect(btnConnect, SIGNAL(clicked()), this, SLOT(onConnect()));

  QSettings settings;
  QString url = settings.value("url").toString();
  if (url.length() < 1)
    return;
  lineEditUrl->setText(url);
  lineEditPort->setText(settings.value("port").toString());
#ifndef FTC
  textEditId->setText(settings.value("id").toString());
#endif
}

void Register::createVerticalGroupBox()
{
  verticalGroupBox = new QGroupBox(APP + " " + VERSION);
  QHBoxLayout *layoutUrl = new QHBoxLayout;
  QHBoxLayout *layoutPort = new QHBoxLayout;
  QHBoxLayout *layoutButton = new QHBoxLayout;
  QVBoxLayout *layout = new QVBoxLayout;

  labelUrl = new QLabel(tr("Url:"));
  lineEditUrl = new QLineEdit(tr("192.168.1.39"));
  labelPort = new QLabel(tr("Port:"));
  lineEditPort = new QLineEdit(tr("1470"));
#ifndef FTC
  QHBoxLayout *layoutId = new QHBoxLayout;
  labelId = new QLabel(tr("UUID:"));
  textEditId = new QTextEdit(tr("0123456789ABCDEF212a7c75af448aa012345678"));
  textEditId->setMaximumHeight(125);

  layoutId->addWidget(labelId);
  layoutId->addWidget(textEditId);
#endif
  btnConnect = new QPushButton(tr("&Connect"));
  layoutUrl->addWidget(labelUrl);
  layoutUrl->addWidget(lineEditUrl);
  layoutPort->addWidget(labelPort);
  layoutPort->addWidget(lineEditPort);
  layoutButton->addWidget(btnConnect);

  layout->addLayout(layoutUrl);
  layout->addLayout(layoutPort);
#ifndef FTC
  layout->addLayout(layoutId);
#endif
  layout->addLayout(layoutButton);

  verticalGroupBox->setLayout(layout);
}

void Register::onConnect()
{
  QString url(lineEditUrl->text());
  if (url.length() < 1) {
    QMessageBox::warning(this, APP,
                          QString("URL can not be empty"),
                          QMessageBox::Yes);
    return;
  }

  bool ok(false);
  int port = lineEditPort->text().toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, APP,
                          QString("Port number should be integer"),
                          QMessageBox::Yes);
    return;
  }
  if ((port < 1000) || (port > 0xFFFF)) {
    QMessageBox::warning(this, APP,
                          QString("Port number should be tween 1000 - 65535"),
                          QMessageBox::Yes);
    return;
  }

#ifndef FTC
  QString id(textEditId->toPlainText());
  if (id.length() != 40) {
    QMessageBox::warning(this, APP,
                          QString("UUID should has 40 characters"),
                          QMessageBox::Yes);
    return;
  }
#endif

  this->hide();
#ifdef FTC
#else
  Chat w(this, lineEditUrl->text(), port, id);
  w.exec();
#endif
  this->show();

  QSettings settings;
  settings.setValue("url", url);
  settings.setValue("port", port);
#ifndef FTC
  settings.setValue("id", id);
#endif
}
