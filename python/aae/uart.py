#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial

port = '/dev/ttyACM0'

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

