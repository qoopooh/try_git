#!/usr/bin/env python
# -*- coding: utf-8 -*-

from kivy.app import App
from console import Console

class ControllerApp(App):

    def build(self):
        return Console(info='Hello World')

if __name__ == '__main__':
    ControllerApp().run()


