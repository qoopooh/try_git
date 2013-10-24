#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, time
from serial import Serial
from aaereader import Reader
from threading import Timer

if os.name == 'posix':
    port = '/dev/ttyACM0'
else:
    port = 'COM4'

packets = [
    ('SetHeartbeat', 1),
    #('GetSerial', None),
    #('GetHardwareRev', None),
    #('GetReaderType', None),
    #('InventoryCyclic', 1),
    #('GetBootloaderRev', None),
    #('GetCurrentState', None),
    #('GetStatusRegister', None),
    #('GetAttenuation', None),
    #('InventorySingle', 1),
    #('InventorySingle', 1),
    #('GetSoftwareRev', None),
    #('InventoryCyclic', 0),
    ('SetHeartbeat', 1),
    ('SetHeartbeat', 0),
    ('SetHeartbeat', 0),
    ('GetSoftwareRev', None),
    ('GetSoftwareRev', None),
    ('GetSoftwareRev', None),
    ('SetHeartbeat', 0),
    ('SetHeartbeat', 0),
    ('SetHeartbeat', 0),
]

def main():
    print "ST110"
    finish = False
    s = Serial(port=port, baudrate=115200)
    reader = Reader(s)
    reader.run = True
    t0 = time.clock()
    i = 0
    while reader.run:
        reader.exec_()
        t1 = time.clock()
        if (t1 - t0 < 2):
            continue
        t0 = t1
        if i < len(packets):
            if reader.send(packets[i]):
                i += 1
            else:
                print('resend', packets[i][0])
                if reader.resend():
                    i += 1
        else:
            print('end of packets')
            break
    finish = True
    reader.run = not finish

if __name__ == '__main__':
    main()

