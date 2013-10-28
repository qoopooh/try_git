#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, time, sys
from serial import Serial
from aaereader import Reader
from threading import Timer

if os.name == 'posix':
    port = '/dev/ttyACM0'
else:
    port = 'COM4'

packets = [
    ('SetHeartbeat', 1),
    ('GetSerial', None),
    ('GetHardwareRev', None),
    ('GetReaderType', None),
    ('InventoryCyclic', 1),
    ('GetBootloaderRev', None),
    ('GetCurrentState', None),
    ('GetStatusRegister', None),
    ('GetAttenuation', None),
    ('InventorySingle', 1),
    ('InventorySingle', 1),
    ('InventorySingle', 1),
    ('InventorySingle', 1),
    ('InventorySingle', 1),
    ('GetSoftwareRev', None),
    ('InventoryCyclic', 0),
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

def main0():
    print "ST110"
    finish = False
    s = Serial(port=port, baudrate=115200)
    reader = Reader(s)
    reader.run = True
    t0 = time.clock()
    i = 0
    while reader.run:
        out = reader.exec_()
        if out is not None:
            print(out)

        t1 = time.clock()
        if (t1 - t0 < 2):
            continue

        t0 = t1
        if i < len(packets):
            if reader.send(packets[i]):
                i += 1
            else:
                sys.stderr.write('resend {pkt}\n'.format(pkt=packets[i][0]))
        else:
            print('end of packets')
            break

    finish = True
    reader.run = not finish

def main():
    print "ST110"
    finish = False
    s = Serial(port=port, baudrate=115200)
    b = False
    delay = 3
    reader = Reader(s)
    for i in range(5):
        b ^= 1
        reader.set_heartbeat(b)
        time.sleep(delay)
        #reader.run = False


if __name__ == '__main__':
    main()

