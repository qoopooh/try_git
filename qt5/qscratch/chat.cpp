#include <QtWidgets>
#include "chat.h"

Chat::Chat(QWidget *parent) :
    QDialog(parent)
{
  createGridGroupBox();

  QVBoxLayout *mainLayout = new QVBoxLayout;

  mainLayout->addWidget(gridGroupBox);
  setLayout(mainLayout);

  setWindowTitle(tr("Basic Layouts"));
}

Chat::~Chat()
{

}

void Chat::onSend()
{

}

void Chat::createMenu()
{

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
}

