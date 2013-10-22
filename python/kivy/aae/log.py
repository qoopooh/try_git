#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Log from reader
"""
import os

from kivy import resources
from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.gridlayout import GridLayout
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput

class LogScreen(GridLayout):
    def __init__(self, **kwargs):
        super(LogScreen, self).__init__(**kwargs)

        platform = os.sys.platform
        if platform == 'linux2':
            resources.resource_add_path('/usr/share/fonts/truetype/droid/')
        elif platform == 'darwin':
            resources.resource_add_path('/Library/Fonts')
        self.cols = 2
#self.add_widget(Label(text=u'User',font_name='DroidSans.ttf'))
        self.add_widget(Label(text=u'ชื่อผู้ใช้งาน',font_name='DroidSansThai.ttf'))
        self.username = TextInput(multiline=False)
        self.add_widget(self.username)
#self.add_widget(Label(text=u'Passwd',font_name='DroidSans.ttf'))
        self.add_widget(Label(text=u'รหัสผ่าน',font_name='DroidSansThai.ttf'))
        self.password = TextInput(password=True, multiline=False)
        self.add_widget(self.password)

class TestApp(App):
    def build(self):
        return LogScreen()

def main():
    TestApp().run()

if __name__ == '__main__':
    main()

