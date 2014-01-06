#include <QtWidgets>
#include "chat.h"

Chat::Chat(QWidget *parent) :
    QDialog(parent)
{
  createHorizontalGroupBox();
  createGridGroupBox();

  QVBoxLayout *mainLayout = new QVBoxLayout;

  mainLayout->addWidget(horizontalGroupBox, 1);
  mainLayout->addWidget(gridGroupBox, 10);
  setLayout(mainLayout);

  setWindowTitle(tr("Basic Layouts"));
}

Chat::~Chat()
{

}

void Chat::onSend()
{

}

void Chat::createHorizontalGroupBox()
{
  horizontalGroupBox = new QGroupBox(tr("Horizontal layout"));
  QHBoxLayout *layout = new QHBoxLayout;
  lineEditCmd = new QLineEdit(tr("0,E,V"));
  btnSend = new QPushButton(tr("Send"));
  btnClear = new QPushButton(tr("Clear"));

  layout->addWidget(lineEditCmd, 4);
  layout->addWidget(btnSend, 1);
  layout->addWidget(btnClear, 1);

  horizontalGroupBox->setLayout(layout);
}

void Chat::createGridGroupBox()
{
  gridGroupBox = new QGroupBox(tr("Grid layout"));
  QGridLayout *layout = new QGridLayout;

  for (int i = 0; i < NumGridRows; ++i) {
    labels[i] = new QLabel(tr("Line %1:").arg(i + 1));
    lineEdits[i] = new QLineEdit;
    layout->addWidget(labels[i], i + 1, 0);
    layout->addWidget(lineEdits[i], i + 1, 1);
  }
  gridGroupBox->setLayout(layout);
}

