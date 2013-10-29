#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, types
from serial import Serial
from threading import Thread
from datetime import datetime

from kivy.app import App
from kivy.config import ConfigParser
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty
from kivy.lang import Builder
from kivy.clock import Clock

from hb import Heartbeat
from aaereader import Reader
from aae import print_hex

if os.name == 'nt': #sys.platform == 'win32':
    from serial.tools.list_ports_windows import *
elif os.name == 'posix':
    from serial.tools.list_ports_posix import *

Builder.load_string("""
[BigBtn@Button]:
    text: ctx.text if hasattr(ctx, 'text') else ''
    text_size: self.size
    font_size: '25sp'
    halign: 'center'
    valign: 'middle'
    markup: True

[ThaiBtn@Button]:
    text: ctx.text if hasattr(ctx, 'text') else ''
    text_size: self.size
    font_name: 'DroidSansThai.ttf'
    font_size: '25sp'
    halign: 'center'
    valign: 'middle'
    markup: True

<Inventory>:
    spacing: 5
    padding: [5, 3, 3, 3]

    txt_inpt: txt_inpt
    btn_toggle: _btn_toggle
    btn_inventory: _btn_inventory
    btn_readfromtag: _btn_readfromtag
    btn_setting: _btn_setting
    btn_clear: _btn_clear
    btn_quit: _btn_quit
    lb_status: _lb_status

    BoxLayout:
        size_hint: (.4, 1)
        orientation: 'vertical'
        spacing: 3

        ThaiBtn:
            id: _btn_toggle
            text: 'ทดสอบ'
        BigBtn:
            id: _btn_inventory
            text: 'Inventory'
        BigBtn:
            id: _btn_readfromtag
            text: 'Read Tag'
        BigBtn:
            id: _btn_setting
            text: 'Settings'
#Image:
#source: "atlas://images/hb/hb_on"
        Label:
            id: _lb_status
            text: 'STANDBY'
        BigBtn:
            id: _btn_clear
            text: 'Clear Screen'
        BigBtn:
            id: _btn_quit
            text: 'Exit'

    TextInput:
        id: txt_inpt
#text: _btn_inventory.state
#on_txtt: root.check_status(_btn_inventory)

""")

class ReaderTask():

    busy = False

    def __init__(self, port):
        s = Serial(port=port, baudrate=115200)
        r = Reader(s)
        r.inventory(self.on_txt)



class Inventory(BoxLayout):

    txt_inpt = ObjectProperty(None)
    f_running = False

    def __init__(self, app, **kwargs):
        super(Inventory, self).__init__(**kwargs)
        self._app = app
        self.btn_inventory.bind(on_release=self.on_excute)
        self.btn_readfromtag.bind(on_release=self.on_excute)
        self.btn_setting.bind(on_release=self._app.open_settings)
        self.btn_clear.bind(on_release=lambda instance: \
                setattr(self.txt_inpt, 'text', ""))
        self.btn_quit.bind(on_release=self.quit)
        self.set_status(self.selected_port())
        #Clock.schedule_interval(self.cb, 1)

    def check_status(self, btn):
        print('button state is: {state}'.format(state=btn.state))

    def hb(self, clk=None):
        if self.btn_toggle.text != 'ทดสอบ':
            self.btn_toggle.text = 'ทดสอบ'
        else:
            self.btn_toggle.text = 'อาร์เอฟไอดี'

    def selected_port(self):
        return self._app.config.get('section1', 'reader')

    def set_status(self, status):
        self.lb_status.text = status

    def _inventory(self, *args, **kwargs):
        try:
            s = Serial(port=args[0], baudrate=115200)
            r = Reader(s, self.hb)
            r.inventory(cb=self.on_txt)
        finally:
            self.f_running = False
            self.set_status('Closed ' + self.selected_port())

    def _readfromtag(self, *args, **kwargs):
        membank = self._app.config.getint('section1', 'membank')
        length = self._app.config.getint('section1', 'length')
        startaddr = int(self._app.config.getint('section1', 'startaddr'))
        try:
            s = Serial(port=args[0], baudrate=115200)
            r = Reader(s, self.hb)
            tags = r.inventory(close=False)
            if len(tags) > 0:
                epc = tags[0]
                r.read_from_tag(epc=epc, membank=membank, length=length,
                        startaddr=startaddr, cb=self.on_txt)
            else:
                r.run = False
        finally:
            self.f_running = False
            self.set_status('Closed ' + self.selected_port())

    def on_excute(self, btn):
        if self.f_running:
            return
        if btn is self.btn_inventory:
            target = self._inventory
        elif btn is self.btn_readfromtag:
            target = self._readfromtag
        else:
            return
        port = self.selected_port()
        self.set_status('Reading on ' + port)
        thread = Thread(target=target, args=(port,))
        thread.daemon = True
        thread.start()
        self.f_running = True

    def on_txt(self, t):
        if len(t) < 1:
            return
        text = ""
        if isinstance(t[0], types.IntType):
            data = print_hex(t)
            text += datetime.now().strftime('%H:%M:%S') + ' -> ' + data + '\n'
        else:
            for tag in t:
                epc = print_hex(tag)
                text += datetime.now().strftime('%H:%M:%S') + ' -> ' + epc + '\n'
        if len(text) > 1:
            self.txt_inpt.text = text + self.txt_inpt.text

    def quit(self, btn):
        self._app.stop()

class InventoryApp(App):

    #use_kivy_settings = False
    icon = 'Logo_only.ico'

    def build_config(self, config):
        config.setdefaults('section1', {
            'reader': 'COM1',
            'attenuation': 0,
            'membank': 2,
            'length': 0,
            'startaddr': 0,
            'output_path': 'output.json'
        })

    def build_settings(self, settings):
        portlist = '[' + ','.join('"{0}"'.format(p[0])
                for p in sorted(comports())) + ']'
        print(portlist)

        jsondata = """
            [{ "type": "title", "title": "Application Settings" },
            { "type": "options",
                "title": "Reader port",
                "desc": "Select the right COM port that reader connect to",
                "section": "section1",
                "key": "reader",
                "options": """ + portlist + """},
            { "type": "numeric",
                "title": "Power attenuation",
                "desc": "Set power attenuation 0 - 10 dBm",
                "section": "section1",
                "key": "attenuation" },
            { "type": "options",
                "title": "Memory bank of Tag",
                "desc": "Please set the number 0:Reserve, 1:EPC, 2:TID and 3:USER",
                "section": "section1",
                "options": ["0", "1", "2", "3"],
                "key": "membank" },
            { "type": "numeric",
                "title": "Data length",
                "desc": "Length of data for Read Tag command",
                "section": "section1",
                "key": "length" },
            { "type": "numeric",
                "title": "Start Address ",
                "desc": "Word (2 bytes) offset for reading/writing tag",
                "section": "section1",
                "key": "startaddr" },
            { "type": "string",
                "title": "Output file path",
                "desc": "Reader output",
                "section": "section1",
                "key": "output_path" }
            ]"""

        settings.add_json_panel('Biomin RFID',
                self.config, data=jsondata)

    def on_config_change(self, config, section, key, value):
        if config is self.config:
            token = (section, key)
            if token == ('section1', 'reader'):
                print('Reader port has been change to', value)
            elif token == ('section1', 'attenuation'):
                print('Power attenuation has been change to', value)
                if 0 <= value <= 10:
                    pass
                else:
                    new_value = 10
                    if float(value) < 0:
                        new_value = 0
                    self.config.set('section1', 'attenuation', new_value)
                    self.config.write()
                    self.build_config(self.load_config())
                    print('over range', value, type(value), new_value)
                print(type(self.config))
            elif token == ('section1', 'output_path'):
                print('Output path has been change to', value)


    def build(self):
        return Inventory(self)

def main():
    InventoryApp().run()

if __name__ == '__main__':
    main()

