#!/usr/bin/env python

import time
from Queue import Queue

from aae import Protocol, Tx, Rx, AAE_COMMAND

class Reader():
    def __init__(self, interface):
        self.i = interface
        self.i.close()
        self.rx = Rx(self.i, self)
        self.rx.daemon = True
        self._tx = Tx(self.i)

    def start(self):
        self.i.open()
        self.rx.start()

    def stop(self):
        self.i.close()

    def exec_(self):
        while self.rx.q.qsize():
            self._rx_buff.append(ord(self.rx.q.get()))
        while (len(self._rx_buff) > 9):
            res = self._p.extract(self._rx_buff)
            if res[0]:
                self._q_packet.put(res[1])
                print ('exec', res[1][0], self._q_packet.qsize()),
            self._rx_buff = self._rx_buff[res[2]:]

            print (';') # new line

        if self._q_packet.qsize():
            packet = self._q_packet.get()
            if packet[0] is 'HeartbeatInt':
                self._hb_count += 1
                if self._hb_count % 5 is 0:
                    print('hb', self._hb_count),
            else:
                self.get_response(packet)
            self._sleep = 1.0
        else:
            if (self._sleep < 50):
                self._sleep += 1
            time.sleep(self._sleep / 100)

        self._tx.exec_()

    def get_response(self, packet):
        command, payload = packet
        self._tx.get_response(command, payload)
    def send(self, packet):
        return self._tx.send(packet)

    def read_from_tag(epc, bank=2, length=0):
        pass

    def write_to_tag(epc, data, bank=3):
        pass

    def set_heartbeat(on):
        pass

    _q_packet = Queue()
    _p = Protocol()
    _rx_buff = []
    _hb_count = 0
    _sleep = 1.


class CommandHandler():
    pass

