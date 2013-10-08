#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial, os

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
    

if __name__ == '__main__':
    main()
