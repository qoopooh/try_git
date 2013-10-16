#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial, os, time
from collections import deque
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
    sleep_counter = 0
    while True:
        n = ser.inWaiting()
        if not n:
            sleep_counter += 1
            if (sleep_counter > 10):
                sleep_counter = 0
                time.sleep(0.1)
            continue
        sleep_counter = 0
        data += ser.read(n)
        out = str(time.clock()) + ":".join("{0:02x}".format(ord(c)) for c in data) 
        print(out), # continue on the same line

        while (len(data) > 9):
            res = p.extract([ord(c) for c in data])
            print (res),
            if (res[0]):
                sender.checkResponse(res[1])
            data = data[res[2]:]

            print (',') # new line
            if not res[2]:
                break

if __name__ == '__main__':
    main()

