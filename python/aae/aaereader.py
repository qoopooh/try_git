#!/usr/bin/env python

import time
from Queue import Queue

from aae import Protocol, Tx, Rx, get_payload_out

class Reader(object):

    def __init__(self, interface):
        self._i = interface
        self._i.close()
        self._rx = Rx(self._i, self)
        self._rx.daemon = True
        self._tx = Tx(self._i)

    @property
    def run(self):
        return self._run

    @run.setter
    def run(self, value):
        if value is not False:
            value = True
        if value:
            self._i.open()
            self._rx.start()
        else:
            self._i.close()
            self._rx.join()
        self._run = value

    def exec_(self):
        try:
            b = self._rx.q.get(block = True, timeout = 0.005)
            self._rx_buff.append(ord(b))
            n = self._rx.q.qsize()
        except:
            #print('TO rx.q.get()')
            return

        while n:
            n -= 1
            self._rx_buff.append(ord(self._rx.q.get()))

        while (len(self._rx_buff) > 9):
            res = self._p.extract(self._rx_buff)
            if res[0]:
                self._get_response(res[1])
                self._tx.exec_()
            else:
                print('failed msg extraction')
            self._rx_buff = self._rx_buff[res[2]:]

    def _get_response(self, packet):
        command, payload = packet
        self._tx.get_response(command, payload)
        resp = get_payload_out(command, payload)
        if command is 'InventoryCyclicInt':
            print('ici', resp)

    def send(self, packet):
        self._sending_packet = packet
        return self._tx.send(packet)
        
    def resend(self):
        self._tx.clear()
        return self._tx.send(self._sending_packet)

    def read_from_tag(bank=2, length=0, epc=None):
        pass

    def write_to_tag(data, bank=3, epc=None):
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

