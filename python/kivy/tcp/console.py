import os
from types import *
from datetime import datetime
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.lang import Builder

from client import Client

path = os.path.dirname(__file__)
path = os.path.join(path, 'console.kv')
Builder.load_file(path)

err_cannot_send = 'Cannot send data, please try to reconnect'
err_cannot_connect = 'Cannot create connection, please try again'


class Console(BoxLayout):

    def __init__(self, app, **kwargs):
        super(Console, self).__init__(**kwargs)
        self._app = app
        self.btn_send.bind(on_release=self.on_send)
        self.btn_clear.bind(on_release=self.on_clear)
        self._c = Client(self.print_rcv)
        self.connect()

    def connect(self):
        ip = self._app.config.get('section1', 'ip')
        port = self._app.config.getint('section1', 'port')
        if not self._c.connect(ip, port):
            pass
#self.show_popup(title='Connection failed', msg=err_cannot_connect)

        format_rx = self._app.config.get('section1', 'format_rx')
        format_tx = self._app.config.get('section1', 'format_tx')
        nl = self._app.config.get('section1', 'newline')
        self.set_format(rx=format_rx, tx=format_tx)
        self.set_newline(nl)
        return self._c.connecting

    def reconnect(self, ip=None, port=None):
        if not self._c.reconnect(ip, port):
            pass
#self.show_popup(title='Connection failed', msg=err_cannot_connect)

    def set_format(self, rx=None, tx=None):
        if rx is not None:
            self._c.rx_hex = True if rx == 'HEX' else False

        if tx is not None:
            self._c.tx_hex = True if tx == 'HEX' else False

    def set_newline(self, n):
        if n is 'LF':
            self._c.newline = '\n'
        elif n is 'CR':
            self._c.newline = '\r'
        elif n is 'CRLF':
            self._c.newline = '\r\n'
        else:
            self._c.newline = None

    def print_rcv(self, msg):
        if isinstance(msg, StringType):
            text = datetime.now().strftime('%H:%M:%S') + ' -> ' + msg + '\n'
        else:
            text = datetime.now().strftime('%H:%M:%S') + ' -> ' + msg[0] \
                  + ': ' + msg[1] + '\n'
        self.txt_monitor.text = text + self.txt_monitor.text

    def on_send(self, btn):
        if self._c.connecting:
            text = self.txt_tx.text
            sz = self._c.send(text)
            self.show_popup('Sent', text)
        else:
            self.show_popup('Sendig failed', err_cannot_send)

    def on_clear(self, btn):
        self.txt_monitor.text = ''

    def show_popup(self, title, content):
        self.print_rcv((title, content))
            

