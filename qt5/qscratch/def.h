#ifndef DEF_H
#define DEF_H

//#define FTC

#ifdef FTC
const QString APP("A-touch FTC");
const QString APP_TITLE("FTC over IP");
const QString VERSION("V1.01");
#else
const QString APP("A-touch Command Line");
const QString APP_TITLE("Domotics Gateway");
const QString VERSION("V0.1");
#endif

#endif // DEF_H
