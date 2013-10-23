#!/usr/bin/env python

import os

from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty
from kivy.lang import Builder


if os.name == 'nt': #sys.platform == 'win32':
    from serial.tools.list_ports_windows import *
elif os.name == 'posix':
    from serial.tools.list_ports_posix import *

Builder.load_string("""
[BigBtn@Button]:
    text: ctx.text if hasattr(ctx, 'text') else ''
    text_size: self.size
    font_size: '25sp'
    markup: True

<Test>:
    txt_inpt: txt_inpt
    BigBtn:
        id: f_but
        text: root.set_btn_text()

    TextInput:
        id: txt_inpt
        text: f_but.state
        on_text: root.check_status(f_but)

    BigBtn:
        id: f_but_quit
        text: 'Quit'
        on_press: root.quit()
""")

class Test(BoxLayout):

    txt_inpt = ObjectProperty(None)

    def __init__(self, config, **kwargs):
        super(Test, self).__init__(**kwargs)
        self._config = config

    def check_status(self, btn):
        print('button state is: {state}'.format(state=btn.state))

    def set_btn_text(self):
        return 'text'
        
    def quit(self):
        print('Bye')

class InventoryApp(App):

    #use_kivy_settings = False

    def build_config(self, config):
        config.setdefaults('section1', {
            'key1': 'value1',
            'key2': '42',
            'reader': 'COM1',
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
            { "type": "string",
                "title": "Output file path",
                "desc": "Reader output",
                "section": "section1",
                "key": "output_path" },
            { "type": "numeric",
                "title": "My second key",
                "desc": "Description of my first key",
                "section": "section1",
                "key": "key2" }
            ]"""

        settings.add_json_panel('Biomin RFID',
                self.config, data=jsondata)

    def on_config_change(self, config, section, key, value):
        if config is self.config:
            token = (section, key)
            if token == ('section1', 'key1'):
                print('Our key1 have been change to', value)
            elif token == ('section1', 'key2'):
                print('Our key2 have been change to', value)


    def build(self):
        return Test(self.config)

def main():
    InventoryApp().run()

if __name__ == '__main__':
    main()

