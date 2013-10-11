#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial, os, time
from aae import Protocol, Sender

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

def sendCommands(packet):
    if not packet or len(packet) < 10:
        return
    
def main():
    print "ST110"
    time.sleep(1)
    ser = serial.Serial(port=port, baudrate=115200)
    print "Reading on " + ser.portstr
    p = Protocol()
    sender = Sender(ser)
    data = residual = []
    while True:
        n = ser.inWaiting()
        if not n:
            continue
        data += ser.read(n)
        print str(time.clock()) + ":".join("{0:02x}".format(ord(c)) for c in data)
        while (len(data) > 9):
            res = p.extract([ord(c) for c in data])
            print (res)
            sender.checkResponse(res[1])
            data = data[res[2]:]
            if not res[2]:
                break

if __name__ == '__main__':
    main()

