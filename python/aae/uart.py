#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, time
from serial import Serial
from Queue import Queue
from aae import Protocol
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


def main1():
    print "ST110"
    time.sleep(1)
    ser = Serial(port=port, baudrate=115200)
    print "Reading on " + ser.portstr
    p = Protocol()
    sender = Sender(ser)
    data = residual = []
    sleep_counter = 0
    q_packet = Queue()
    q_rx = Queue()
    hb_count = 0
    while True:
        n = ser.inWaiting()
        if not n:
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
                q_packet.put(res[1])
            data = data[res[2]:]

            print (',') # new line
            if not res[2]:
                break

        if q_packet.qsize():
            packet = q_packet.get()
            if packet[0] is 'HeartbeatInt':
                hb_count += 1
                d = hb_count / 3
                if d < len(packets) and hb_count % 3 is 0:
                    sender.send(packets[d])
            sender.get_response(packet)
        sender.exec_()

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

