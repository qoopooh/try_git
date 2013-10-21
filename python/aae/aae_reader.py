#!/usr/bin/env python

import time
from Queue import Queue

from aae import Protocol, Tx, Rx, get_payload_out

class Reader(object):

    def __init__(self, interface):
        self.i = interface
        self.i.close()
        self._rx = Rx(self.i, self)
        self._rx.daemon = True
        self._tx = Tx(self.i)

    @property
    def run(self):
        return self._run

    @run.setter
    def run(self, value):
        if value is not False:
            value = True
        if value:
            self.i.open()
            self._rx.start()
        else:
            self.i.close()
        self._run = value

    def start(self):
        self.i.open()
        self._rx.start()

    def stop(self):
        self.i.close()

    def exec_(self):
        while self._rx.q.qsize():
            self._rx_buff.append(ord(self._rx.q.get()))
        while (len(self._rx_buff) > 9):
            res = self._p.extract(self._rx_buff)
            if res[0]:
                self._q_packet.put(res[1])
                pass #print ('put', res[1][0], self._q_packet.qsize()),
            self._rx_buff = self._rx_buff[res[2]:]

            pass #print (';') # new line

        self._check_q_packet()

        self._tx.exec_()

    def _check_q_packet(self):
        if self._q_packet.qsize():
            packet = self._q_packet.get()
            if packet[0] is 'HeartbeatInt':
                self._hb_count += 1
                if self._hb_count % 5 is 0:
                    pass #print('hb', self._hb_count),
            else:
                self.get_response(packet)
            self._sleep = 1.0
        else:
            if (self._sleep < 50):
                self._sleep += 1
            time.sleep(self._sleep / 100)

    def get_response(self, packet):
        command, payload = packet
        self._tx.get_response(command, payload)
        resp = get_payload_out(command, payload)
        if command is 'InventoryCyclicInt':
            print('ici', resp)

    def send(self, packet):
        while not self._tx.send(packet): pass
        return self._tx.last_result == 'success'

    def read_from_tag(epc, bank=2, length=0):
        pass

    def write_to_tag(epc, data, bank=3):
        pass

    def set_heartbeat(on):
        send('SetHeartbeat', on)
        

    _q_packet = Queue()
    _p = Protocol()
    _rx_buff = []
    _hb_count = 0
    _sleep = 1.
    _run = False


class CommandHandler():
    pass

