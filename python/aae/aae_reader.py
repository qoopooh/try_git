#!/usr/bin/env python

import time
from threading import Thread, Event
from Queue import Queue

from aae import Protocol, Sender


class Rx(Thread):
    def __init__(self, interface, parent):
        Thread.__init__(self)
        self.i = interface
        self.q = Queue()
        self.s_count = 0
        self._parent = parent

    def run(self):
        while self.i.isOpen() and self._parent:
            n = self.i.inWaiting()
            if n < 1:
                self.s_count += 1
                if (self.s_count > 5):
                    self.s_count = 0
                    time.sleep(0.02)
                continue

            selfks_count = 0
            data = self.i.read(n)
            print('rx_get', len(data), 'last_q', self.q.qsize())
            for d in data:
                self.q.put(d)

class Reader():
    def __init__(self, interface):
        self.i = interface
        self.i.close()
        self.rx = Rx(self.i, self)
        self.rx.daemon = True
        self.tx = Sender(self.i)
        self.rx_buff = []
        self.q_packet = Queue()
        self.p = Protocol()
        self.hb_count = 0

    def start(self):
        self.i.open()
        self.rx.start()

    def stop(self):
        self.i.close()

    def exec_(self):
        while self.rx.q.qsize():
            self.rx_buff.append(self.rx.q.get())
        while (len(self.rx_buff) > 9):
            res = self.p.extract([ord(c) for c in self.rx_buff])
#res = self.p.extract(self.rx_buff)
            print (res),
            if (res[0]):
                self.q_packet.put(res[1])
            self.rx_buff = self.rx_buff[res[2]:]

            print (';') # new line
            if not res[2]:
                break

        if self.q_packet.qsize():
            ''' check received packet '''
            packet = self.q_packet.get()
            if packet[0] is 'HeartbeatInt':
                self.hb_count += 1
                if self.hb_count % 5 is 0:
                    print('hb', self.hb_count),
            self.tx.get_response(packet)
        self.tx.exec_()

    def send(self, packet):
        return self.tx.send(packet)



