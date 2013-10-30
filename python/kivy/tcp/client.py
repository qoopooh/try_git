#!/usr/bin/env python

import time

from socket import socket, AF_INET, SOCK_STREAM
from threading import Thread
from datetime import datetime

class Client():

    def __init__(self, recv_cb=None):
        self._recv_cb = recv_cb
        self._s = socket(AF_INET, SOCK_STREAM)
        self._host = '192.168.1.92'
        self._port = 1470
        self._connect = False

    @property
    def connecting(self):
        return self._connect

    def reconnect(self, ip=None, port=None):
        self._s.close()
        self.connect(ip, port)

    def connect(self, ip=None, port=None):
        if ip is not None:
            self._host = ip
        if port is not None:
            self._port = port
        self._s.connect((self._host, self._port))
        self._rx = Thread(target=self.receiving)
        self._rx.daemon = True
        self._rx.start()
        self._connect = True

    def close(self):
        self._s.close()
        self._connect = False

    def receiving(self):
        chunk = self._s.recv(1024)
        while len(chunk) > 0:
            if self._recv_cb is not None:
                self._recv_cb(chunk)
            chunk = self._s.recv(1024)
        self.close()

    def send(self, data):
        sz = 0
        if self._connect:
            sz = self._s.send(data)
        return sz

    def __del__(self):
        self._s.close()

def print_data(data):
    print datetime.now(), len(data), data

if __name__ == '__main__':
    c = Client(print_data)
    c.connect()
    sz = c.send('Hi')
    count = 0
    while sz > 0:
        time.sleep(1)
        sz = c.send(' {c}'.format(c=count))
        count += 1
        pass

