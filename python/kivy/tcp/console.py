import os
from datetime import datetime
from kivy.uix.boxlayout import BoxLayout
from kivy.lang import Builder

from client import Client

path = os.path.dirname(__file__)
path = os.path.join(path, 'console.kv')
Builder.load_file(path)

class Console(BoxLayout):

    def __init__(self, app, **kwargs):
        super(Console, self).__init__(**kwargs)
        self._app = app
        self._s = Client(self.print_rcv)
        self.connect()

    def connect(self):
        ip = self._app.config.get('section1', 'ip')
        port = self._app.config.getint('section1', 'port')
        self._s.connect(ip, port)

    def reconnect(self, ip=None, port=None):
        self._s.reconnect(ip, port)

    def set_format(self, rx=None, tx=None):
        if rx is not None:
            self._s.rx_hex = True if rx == 'HEX' else False

        if tx is not None:
            self._s.tx_hex = True if tx == 'HEX' else False

    def set_newline(self, n):
        if n is 'LF':
            self._s.newline = '\n'
        elif n is 'CR':
            self._s.newline = '\r'
        elif n is 'CRLF':
            self._s.newline = '\r\n'
        else:
            self._s.newline = None

    def print_rcv(self, data):
        if len(data) > 0:
            text = datetime.now().strftime('%H:%M:%S') + ' -> ' + data + '\n'
        else:
            text = datetime.now().strftime('%H:%M:%S') + ' closed'
        self.txt_monitor.text = text + self.txt_monitor.text


