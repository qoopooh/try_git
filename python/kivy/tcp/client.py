#!/usr/bin/env python

import time, sys

from socket import socket, AF_INET, SOCK_STREAM
from threading import Thread
from datetime import datetime

from sthex import *


class Client(object):

    def __init__(self, recv_cb=None):
        self._recv_cb = recv_cb
        self._s = None
        self._host = '192.168.1.92'
        self._port = 1470
        self._connect = False
        self._rx_hex = True
        self._tx_hex = True

    @property
    def connecting(self):
        return self._connect

    @property
    def host(self):
        return self._host
    @host.setter
    def host(self, val):
        self._host = val

    @property
    def port(self):
        return self._port
    @host.setter
    def port(self, val):
        self._port = val

    @property
    def rx_hex(self):
        return self._rx_hex
    @rx_hex.setter
    def rx_hex(self, val):
        self._rx_hex = val

    @property
    def tx_hex(self):
        return self._tx_hex
    @rx_hex.setter
    def tx_hex(self, val):
        self._tx_hex = val

    @property
    def newline(self):
        r""" Return '\r', '\n' and '\r\n' """
        return self._nl
    @newline.setter
    def newline(self, val):
        self._nl = val

    def reconnect(self, ip=None, port=None):
        self.close()
        return self.connect(ip, port)

    def connect(self, ip=None, port=None):
        if ip is not None:
            self._host = ip
        if port is not None:
            self._port = port
        try:
            self._s = socket(AF_INET, SOCK_STREAM)
            self._s.connect((self._host, self._port))
        except:
            sys.stderr.write('Connecting failed\n')
            return False
        self._rx = Thread(target=self.receiving)
        self._rx.daemon = True
        self._rx.start()
        self._connect = True
        return True

    def close(self):
        try:
            self._s.close()
        except:
            sys.stderr.write('Closing failed\n')
        self._connect = False

    def receiving(self):
        chunk = self._s.recv(1024)
        while len(chunk) > 0:
            if self._recv_cb is not None:
                if self._rx_hex:
                    chunk = hex_to_hexstr(chunk)
                self._recv_cb(chunk)
            try:
                chunk = self._s.recv(1024)
            except:
                break
        self.close()

    def send(self, data):
        sz = 0
        if not self._connect:
            self.connect()
        if self._connect:
            if self._tx_hex:
                sz = self._s.send(bytearray(hexstr_to_hex(data)))
            else:
                sz = self._s.send(data)
        return sz

def print_rcv(data):
    print datetime.now(), len(data), data

if __name__ == '__main__':
    c = Client(print_rcv)
    c.connect()
    c.rx_hex = False
    c.tx_hex = False
    raw ='48-69-20'
    sz = c.send(hex_trim(raw))
    count = 0
    while sz > 0:
        time.sleep(1)
#sz = c.send(' {c}'.format(c=count))
        sz = c.send('203{c}'.format(c=count))
        if count < 9:
            count += 1
        else:
            count = 0

