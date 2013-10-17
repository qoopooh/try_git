#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, time
from serial import Serial
from aae_reader import Reader

if os.name == 'posix':
    port = '/dev/ttyACM0'
else:
    port = 'COM4'

packets = [
    ('GetSerial', None),
    ('GetReaderType', None),
    #('GetBootloaderRev', None),
    #('GetCurrentState', None),
    #('GetStatusRegister', None),
    #('GetAttenuation', None),
    ('InventoryCyclic', 1),
    ('InventorySingle', 1),
    ('GetHardwareRev', None),
    ('InventorySingle', 1),
    ('GetSoftwareRev', None),
    ('InventoryCyclic', 0),
#'GetFrequency',
]


def main():
    print "ST110"
    time.sleep(1)
    s = Serial(port=port, baudrate=115200)
    reader = Reader(s)
    reader.start()
    t0 = time.clock()
    i = 0
    while True:
        reader.exec_()
        t1 = time.clock()
        if (t1 - t0 < 2):
            continue
        t0 = t1
        if i < len(packets) and reader.send(packets[i]):
            i += 1


if __name__ == '__main__':
    main()

