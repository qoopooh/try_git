#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial, os, time
from aae import Protocol

if os.name == 'posix':
    port = '/dev/ttyACM0'
else:
    port = 'COM4'

#def main():
    #print "ST110"
    #time.sleep(3)
    #ser = serial.Serial(port=port, baudrate=115200)
    #print ser.portstr
##while (!ser.readable());
    #a = ser.read(12)
    #ser.close()
    #print 'Closing'
    #print ":".join("{0:02x}".format(ord(c)) for c in a)
    #p = Protocol()
    #res = p.extract([ord(c) for c in a])
    #print res
    
def main():
    print "ST110"
    time.sleep(3)
    ser = serial.Serial(port=port, baudrate=115200)
    print "Reading " + ser.portstr
    p = Protocol()
    data = residual = []
    while True:
        if not ser.inWaiting():
            continue
        data += ser.read()
        print ":".join("{0:02x}".format(ord(c)) for c in data)
        if (len(data) > 9):
            res = p.extract([ord(c) for c in data])
            print (res)
            data = data[res[2]:]

if __name__ == '__main__':
    main()

