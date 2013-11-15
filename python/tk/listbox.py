#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
ZetCode Tkinter tutorial

In this script, we show how to
use the Listbox widget.

author: Jan Bodar
last modified: December 2010
website: www.zetcode.com
"""

from ttk import Frame, Label, Style, Scale
from Tkinter import Tk, BOTH, Listbox, StringVar, END, IntVar


class Example(Frame):

    def __init__(self, parent):
        Frame.__init__(self, parent)

        self.parent = parent
        self.initUI()

    def initUI(self):

        self.parent.title("Listbox + Scale")
        self.pack(fill=BOTH, expand=1)
        acts = ['Scarlett Johansson', 'Rachel Weiss',
            'Natalie Portman', 'Jessica Alba']

        lb = Listbox(self)
        for i in acts:
            lb.insert(END, i)

        lb.bind("<<ListboxSelect>>", self.onSelect)
        lb.place(x=20, y=20)

        self.var = StringVar()
        self.label = Label(self, text=0, textvariable=self.var)
        self.label.place(x=20, y=190)

        scale = Scale(self, from_=0, to=100, command=self.onScale)
        scale.place(x=20, y=220)

        self.var_scale = IntVar()
        self.label_scale = Label(self, text=0, textvariable=self.var_scale)
        self.label_scale.place(x=150, y=220)

    def onSelect(self, val):
        sender = val.widget
        idx = sender.curselection()
        value = sender.get(idx)
        self.var.set(value)

    def onScale(self, val):
        v = int(float(val))
        self.var_scale.set(v)

def main():

    root = Tk()
    ex = Example(root)
    root.geometry("300x250+300+300")
    root.mainloop()


if __name__ == '__main__':
    main()

