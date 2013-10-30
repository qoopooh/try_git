#!/usr/bin/env python
# -*- coding: utf-8 -*-

from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.lang import Builder

Builder.load_file('console.kv')

class Console(BoxLayout):

    def text(self, val):
        print('text input text is: {txt}'.format(txt=val))


class ControllerApp(App):

    def build(self):
        return Console(info='Hello World')

if __name__ == '__main__':
    ControllerApp().run()


