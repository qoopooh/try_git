#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
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
    btn_setting: _btn_setting
    btn_clear: _btn_clear
    btn_quit: _btn_quit

    BoxLayout:
        size_hint: (.4, 1)
        orientation: 'vertical'
        spacing: 3

        BigBtn:
            id: _btn_toggle
            text: 'Message'
        BigBtn:
            id: _btn_inventory
            text: 'Inventory'
        ThaiBtn:
            id: _btn_setting
            text: 'ตั้งค่า'
        Image:
            source: "atlas://images/hb/hb_on"
        Heartbeat:
            id: _hb
        ThaiBtn:
            id: _btn_clear
            text: 'ล้างหน้าจอ'
        ThaiBtn:
            id: _btn_quit
            text: 'ออกโปรแกรม'

    TextInput:
        id: txt_inpt
#text: _btn_inventory.state
#on_text: root.check_status(_btn_inventory)

""")

class ReaderTask():

    busy = False

    def __init__(self, port):
        s = Serial(port=port, baudrate=115200)
        r = Reader(s)
        r.inventory(self.on_tex)



class Inventory(BoxLayout):

    txt_inpt = ObjectProperty(None)

    def __init__(self, app, **kwargs):
        super(Inventory, self).__init__(**kwargs)
        self.app = app
        self.btn_inventory.bind(on_release=self.on_inventory)
        self.btn_setting.bind(on_release=self.app.open_settings)
        self.btn_clear.bind(on_release=lambda instance: \
                setattr(self.txt_inpt, 'text', ""))
        self.btn_quit.bind(on_release=self.quit)
        Clock.schedule_interval(self.cb, 1)

    def check_status(self, btn):
        print('button state is: {state}'.format(state=btn.state))

    def cb(self, clk):
        if self.btn_toggle.text != 'Message':
            self.btn_toggle.text = 'Message'
        else:
            self.btn_toggle.text = 'Toggle'

    def on_inventory(self, btn):
        port =  self.app.config.get('section1', 'reader')
        s = Serial(port=port, baudrate=115200)
        r = Reader(s)
        r.inventory(self.on_tex)

    def on_tex(self, tags):
        print tags

        text = self.txt_inpt.text
        for tag in tags:
            epc = print_hex(tag)
            text += datetime.now().strftime('%H:%M:%S') + ' -> ' + epc + '\n'
        self.txt_inpt.text = text

    def quit(self, btn):
        print('Bye', btn)
        self.app.stop()

class InventoryApp(App):

    #use_kivy_settings = False
    icon = 'Logo_only.ico'

    def build_config(self, config):
        config.setdefaults('section1', {
            'reader': 'COM1',
            'attenuation': 0,
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

