#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial, os, time
#from collections import deque
from Queue import Queue
from aae import Protocol, Sender

if os.name == 'posix':
    port = '/dev/ttyACM0'
else:
    port = 'COM4'

packets = [
    ('GetSerial', None),
    ('GetReaderType', None),
    ('GetHardwareRev', None),
    ('GetSoftwareRev', None),
    #('GetBootloaderRev', None),
    #('GetCurrentState', None),
    #('GetStatusRegister', None),
    #('GetAttenuation', None),
    ('InventoryCyclic', 1),
    ('InventorySingle', 1),
    ('InventorySingle', 1),
    ('InventoryCyclic', 0),
#'GetFrequency',
]


def sendpackets(packet):
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
    queue = Queue()
    hb_count = 0
    while True:
        n = ser.inWaiting()
        if not n:
            if queue.qsize():
                packet = queue.get()
                if packet[0] is 'HeartbeatInt':
                    hb_count += 1
                    d = hb_count / 3
                    if d < len(packets) and hb_count % 3 is 0:
                        sender.send(packets[d])
                sender.get_response(packet)
            sender.exec_()

            sleep_counter += 1
            if (sleep_counter > 10):
                sleep_counter = 0
                time.sleep(0.07)
            continue
        sleep_counter = 0
        data += ser.read(n)
        out = str(time.clock()) + ' ' + ":".join("{0:02x}".format(ord(c)) for c in data) 
        print(out), # continue on the same line

        while (len(data) > 9):
            res = p.extract([ord(c) for c in data])
            print (res),
            if (res[0]):
                queue.put(res[1])
            data = data[res[2]:]

            print (',') # new line
            if not res[2]:
                break

if __name__ == '__main__':
    main()

