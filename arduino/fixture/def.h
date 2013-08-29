#ifndef DEF_H
#define DEF_H

const char FirmwareVersion[5] = "A001";

const int ButtonBootloader = 2;
const int ButtonPower = 3;

const int InputVoltage = 0; /* analog */
const int InputCurrent = 1; /* analog */

const int OutputReaderPower = 4;

const int StatusPowerOn = 12; /* obsoleted */
const int StatusPowerDrop = 5;
const int StatusCurrentError = 7;
const int StatusOnBoard = 13;

const int BAUD_9600 = 9600;
const int BAUD_38400 = 38400;
const int DELAY_BUTTON = 200; /* ms */

const int EepAddrVoltMin = 0;
const int EepAddrVoltMax = 1;
const int EepAddrCurrMin = 2;
const int EepAddrCurrMax = 3;

#endif /* DEF_H */

