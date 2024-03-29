#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Test kivy
http://kivy.org/docs/guide/basic.html
"""
from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.gridlayout import GridLayout
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput

class LoginScreen(GridLayout):
    def __init__(self, **kwargs):
        super(LoginScreen, self).__init__(**kwargs)
        self.cols = 2
        self.add_widget(Label(text='User Name'))
        self.username = TextInput(multiline=False)
        self.add_widget(self.username)
        self.add_widget(Label(text='Password'))
        self.password = TextInput(password=True, multiline=False)
        self.add_widget(self.password)
        
class MyApp(App):

    def build(self):
        return LoginScreen()

def main():
    MyApp().run()

if __name__ == '__main__':
    main()

