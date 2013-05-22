#include "bootloader.h"
#include "XonXoff.h"

BootLoader::BootLoader(QObject *parent) :
    AaeReader(parent)
{
    setBaudrate(115200);
    dataReceivedQueue.clear();
    blReceiveState = BL_RCV_UNDEF;
}

void BootLoader::onReadyRead()
{
    dataReceivedByteArray.clear();
    dataReceivedByteArray = readDataFromConnection();
    responseFlag = true;

    foreach (char ch, dataReceivedByteArray) {
        dataReceivedQueue.enqueue (ch);
    }
    while (dataReceivedQueue.length () > 0) {
        switch (blReceiveState) {
        case BL_RCV_XON:
            if (dataReceivedQueue.dequeue() == XON) {
                blReceiveState = BL_RCV_XOFF;
            }
            break;
        case BL_RCV_XOFF:
            if (dataReceivedQueue.dequeue() == XOFF) {
                blReceiveState = BL_RCV_XON;
                updateFlashProgress();
            }
            break;
        default:
            if (dataReceivedQueue.dequeue() == XON) {
                blReceiveState = BL_RCV_XOFF;
            } else {
                blReceiveState = BL_RCV_XON;
                updateFlashProgress();
            }
            break;
        }
    }
}
void BootLoader::updateFlashProgress()
{
    flashingProgressPercentage = (pageCounter * 100) / pageNumber;
    emit nextpage(flashingProgressPercentage);
    emit raiseStatusMessage(tr("Flashing progress %1%").arg(flashingProgressPercentage));
}

quint16 BootLoader::writeFirmware(const QByteArray &_flashMemory)
{
    pageCounter = 0;
    flashMemory = &_flashMemory;
    pageNumber = flashMemory->size() / FIRMWARE_PAGE_SIZE;

    return pageNumber;
}
bool BootLoader::writeFirmwareStep()
{
    sendDataToReader(flashMemory->mid(pageCounter * FIRMWARE_PAGE_SIZE, FIRMWARE_PAGE_SIZE));

    pageCounter++;
    if (pageCounter > pageNumber) {
        return true;
    }
    return false;
}

