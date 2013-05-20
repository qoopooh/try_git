#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "aaereader.h"

#define FIRMWARE_PAGE_SIZE             0x100    // Pages are 256 bytes long

class BootLoader : public AaeReader
{
    Q_OBJECT
public:
    enum BL_RCV_STATE {
        BL_RCV_XON,
        BL_RCV_XOFF,
        BL_RCV_UNDEF =0xFE
    };

    explicit BootLoader(QObject *parent = 0);

    quint16 writeFirmware(const QByteArray &_flashMemory);
    bool writeFirmwareStep();

signals:
    void nextpage(int progressPercentage);

public slots:

protected slots:
    void onReadyRead();

protected:
    quint16 pageCounter;
    quint16 pageNumber;
    const QByteArray *flashMemory;
    BL_RCV_STATE blReceiveState;
    int flashingProgressPercentage;

    void updateFlashProgress();
};

#endif // BOOTLOADER_H
