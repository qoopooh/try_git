#!/usr/bin/env python
# -*- coding: utf-8 -*-

from Tkinter import Tk, Text, BOTH, W, N, E, S
from ttk import Frame, Button, Label, Style

class ReaderPanel(Frame):

    def __init__(self, parent):
        Frame.__init__(self, parent)
        self.parent = parent
        self.initUI()

    def initUI(self):

        self.parent.title("ST110 Reader V1.00")
        self.style = Style()
        self.style.theme_use("default")
        self.pack(fill=BOTH, expand=1)

        self.columnconfigure(1, weight=1)
        self.columnconfigure(3, pad=7)
        self.rowconfigure(3, weight=1)
        self.rowconfigure(5, pad=7)

        lbl = Label(self, text="Windows")
        lbl.grid(sticky=W, pady=4, padx=5)

        area = Text(self)
        area.grid(row=1, column=0, columnspan=2, rowspan=4,
        padx=5, sticky=E+W+S+N)

        abtn = Button(self, text="Activate")
        abtn.grid(row=1, column=3)

        cbtn = Button(self, text="Close", command=self.close)
        cbtn.grid(row=2, column=3, pady=4)

        hbtn = Button(self, text="Help")
        hbtn.grid(row=5, column=0, padx=5)

        obtn = Button(self, text="OK")
        obtn.grid(row=5, column=3)

    def close(self):
        self.quit()

def main():

    root = Tk()
    root.geometry("350x300+300+300")
    app = ReaderPanel(root)
    root.mainloop()


if __name__ == '__main__':
    main()

