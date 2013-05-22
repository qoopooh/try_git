//-------------------------------
// File:			XonXoff.h
// Function:		XON/XOFF protocol
// Supported Toolchain(s):
//		- Microsoft Visual C++
// Date created:	23-May-2006
// Created by:		JJo
//-------------------------------

#ifndef _XONXOFF_H
#define _XONXOFF_H

//-------------------------------
// Includes

//#include "Communication.h"

//-------------------------------
// Defines

#define XON                     0x11
#define XOFF                    0x13
#define XONXOFF_PACKETSIZE	64
#define XON_XOFF_TIMEOUT	100000

//-------------------------------
// Class

//class XonXoff : public Communication {
class XonXoff {
public:
        XonXoff();
        ~XonXoff();

        unsigned long Send(void * data,
                unsigned long size,
//                ProgressDialog * pProgressDialog,
                unsigned long rate);
};

#endif
