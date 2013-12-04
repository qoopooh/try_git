#ifndef STREADER_H
#define STREADER_H

#include "aaereader.h"
#include "aaecommand.h"

class StReader : public AaeReader
{
    Q_OBJECT
public:
    StReader(QObject *parent = 0);
    void sendTestData();
    void getAttenuation();
    void inventorySingle();
    void inventoryCyclic(bool);
    bool gotoBootloaderMode();
    bool gotoFirmwareMode();
    void setUsbReader(bool);
    AaeCommand::AAE_COMMAND getCommandName();

signals:
    void heartbeatSignal();
    void stx10BootloaderMode(bool blMode);

protected slots:
    virtual void onReadyRead();

protected:
    AaeCommand *aaeCommand;
    AaeCommand::AAE_COMMAND aaeCommandName;
    qint8 debugSequenceCounter;
    bool blMode;
    bool usb;

    void emitCommand(AaeCommand::AAE_COMMAND cmdName);
};

#endif // STREADER_H

