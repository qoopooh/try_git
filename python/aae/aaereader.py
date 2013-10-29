#!/usr/bin/env python

import time
from Queue import Queue, Empty

from aae import Protocol, Tx, Rx, get_command_data

class Reader(object):

    _run = False

    def __init__(self, interface, hb=None):
        self._i = interface
        self._i.close()
        self._tx = Tx(self._i)
        self._rcv_packet = ()
        self.hb_callback = hb

    @property
    def run(self):
        return self._run

    @run.setter
    def run(self, value):
        if value is not False:
            value = True
        if value == self._run:
            return
        self._run = value
        if value:
            self._i.open()
            self._rx = Rx(self._i)
            self._rx.daemon = True
            self._rx.start()
        else:
            self._i.close()
            self._rx.join()

    def exec_(self):
        out = None
        try:
            self._rcv_packet = self._rx.q.get(block = True, timeout = 0.005)
        except Empty:
            self._rcv_packet = None
        if self._rcv_packet is not None:
            out = self._get_response(self._rcv_packet)
        self._tx.exec_()
        return out

    def _get_response(self, packet):
        command, payload = packet
        command_data = get_command_data(command, payload)
        self._tx.get_response(command, command_data)
        if command is 'InventoryCyclicInt':
            print(command, command_data)
        if command is 'HeartbeatInt':
            if self.hb_callback is not None:
                self.hb_callback()
        return command, command_data

    def send(self, packet):
        self._sending_packet = packet
        return self._tx.send(packet)

    def resend(self):
        self._tx.clear()
        return self._tx.send(self._sending_packet)

    def set_heartbeat(self, on):
        if not self.run:
            self.run = True
        self.send(('SetHeartbeat', on))
        while self._tx.busy: self.exec_()

    def inventory(self, close=True, cb=None):
        tags = ()
        cmd = 'InventorySingle'
        if not self.run:
            time.sleep(0.5)
            self.run = True
        packet = (cmd, True)
        self.send(packet)
        repeat = 0
        while True:
            while self._tx.busy:
                o = self.exec_()
                if o is not None and o[0] is cmd:
                    tags = o[1]
            if len(tags) < 1 and repeat < 5:
                repeat += 1
                time.sleep(.8)
                self.send(packet)
            else:
                break

        if close:
            self.run = False
        if cb is not None:
            cb(tags)
        else:
            return tags

    def read_from_tag(self, epc, membank=2, length=0, startaddr=0,
            password=0, cb=None):
        data = ()
        cmd = 'ReadFromTag'
        if not self.run:
            time.sleep(0.5)
            self.run = True
        payload = [len(epc)]
        payload.extend(epc)
        payload.append(membank)
        payload.extend((startaddr >> 16 & 0xFF,startaddr & 0xFF))
        payload.extend(password >> i & 0xFF for i in (24, 16, 8, 0))
        payload.append(length)
        packet = (cmd, payload)

        self.send(packet)
        repeat = 0
        while self._tx.busy:
            o = self.exec_()
            if o is not None and o[0] is cmd:
                data = o[1]
                break

        self.run = False
        if len(data) > 2:
            data = data[2:]
        else:
            data = ()
        if cb is not None:
            cb(data)
        else:
            return data



class CommandHandler():
    pass

