#!/usr/bin/env python
# -*- coding: utf-8 -*-

from kivy.app import App
from console import Console

class MainApp(App):

    use_kivy_settings = False
    #icon = 'Logo_only.ico'

    def build_config(self, config):
        config.setdefaults('section1', {
            'ip': '192.168.1.31',
            'port': 1470,
            'format_rx': 'HEX',
            'format_tx': 'HEX',
            'newline': 'CRLF',
        })

    def build_settings(self, settings):
        jsondata = """
            [{ "type": "title", "title": "Socket Settings" },
            { "type": "string",
                "title": "Server IP",
                "desc": "IP address of TCP server",
                "section": "section1",
                "key": "ip" },
            { "type": "numeric",
                "title": "Server Port",
                "desc": "TCP socket port",
                "section": "section1",
                "key": "port" },
            { "type": "options",
                "title": "Receiver Format",
                "desc": "Data communication format",
                "section": "section1",
                "options": ["HEX", "ASCII"],
                "key": "format_rx" },
            { "type": "options",
                "title": "Sender Format",
                "desc": "Data communication format",
                "section": "section1",
                "options": ["HEX", "ASCII"],
                "key": "format_tx" },
            { "type": "options",
                "title": "New Line",
                "desc": "New line splitter for ASCII format",
                "section": "section1",
                "options": ["None", "CR", "LF", "CRLF"],
                "key": "newline" }
            ]"""

        settings.add_json_panel('TCP Client',
                self.config, data=jsondata)

    def on_config_change(self, config, section, key, value):
        if config is self.config:
            token = (section, key)
            if token == ('section1', 'ip'):
                self._console.reconnect(ip=value)
            elif token == ('section1', 'port'):
                self._console.reconnect(port=int(value))
            elif token == ('section1', 'format_rx'):
                self._console.set_format(rx=value)
            elif token == ('section1', 'format_tx'):
                self._console.set_format(tx=value)
            elif token == ('section1', 'newline'):
                self._console.set_newline(value)

    def build(self):
        self._console = Console(self)
        return self._console

if __name__ == '__main__':
    MainApp().run()


