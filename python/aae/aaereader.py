#!/usr/bin/env python

import time
from Queue import Queue

from aae import Protocol, Tx, Rx, get_payload_out

class Reader(object):

    def __init__(self, interface):
        self._i = interface
        self._i.close()
        self._rx = Rx(self._i)
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
            packet = self._rx.q.get(block = True, timeout = 0.005)
        except:
            #print('TO rx.q.get()')
            pass
        self._get_response(packet)
        self._tx.exec_()

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

    _run = False


class CommandHandler():
    pass

