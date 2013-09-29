#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Test kivy
http://kivy.org/docs/tutorials/firstwidget.html
"""
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.graphics import Color, Ellipse

class PaintWidget(Widget):

    def on_touch_down(self, touch):
        with self.canvas:
            Color(1, 1, 0)
            d = 30.
            Ellipse(pos=(touch.x - d / 2, touch.y - d / 2), size=(d, d))

class PaintApp(App):

    def build(self):
        return PaintWidget()

def main():
    PaintApp().run()

if __name__ == '__main__':
    main()

