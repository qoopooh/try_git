#ifndef PRREADER_H
#define PRREADER_H

#include "aaereader.h"

class PrReader : public AaeReader
{
    Q_OBJECT
public:
    typedef enum PR_COMMAND {
        CmdGetFirmwareVersion00,
        CmdGetHardwareVersion01,
        CmdGetReaderId02,
        CmdGetReaderProtocol0A,
        CmdGetReaderType0B,
        CmdGetStSoftwareRevision0C,
        CmdSetBaudrateE4,
        CmdSetStx10DirectModeE5,
        CmdResetPrx10F2,
        CmdUndef = 0xFE
    }PR_COMMAND;
    explicit PrReader(QObject *parent = 0);

protected slots:
    void onReadyRead();

protected:
    PR_COMMAND prCommand;

    void processCommand();
    PrReader::PR_COMMAND getCommand(const QByteArray &_ba);
    void emitCommand(PrReader::PR_COMMAND prCommand);
};

#endif // PRREADER_H
