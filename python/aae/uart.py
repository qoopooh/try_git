#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial, os
from aae import Protocol

if os.name == 'posix':
    port = '/dev/ttyACM0'
else:
    port = 'COM4'

def main():
    ser = serial.Serial(port=port, baudrate=115200)
    print ser.portstr
#while (!ser.readable());
    a = ser.read(12)
    ser.close()
    print 'Closing'
    print ":".join("{0:02x}".format(ord(c)) for c in a)
    p = Protocol()
    res = p.extract([ord(c) for c in a])
    print res
    

if __name__ == '__main__':
    main()

