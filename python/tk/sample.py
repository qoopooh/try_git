#!/usr/bin/env python

from Tkinter import Tk, BOTH, Frame, Button, Label
from PIL import Image, ImageTk

class App(Frame):

    def __init__(self, parent):
        Frame.__init__(self, parent, background='blue')
        self.parent = parent
        self.initUI()

    def say_hi(self):
        print "hi there, everyone!"

    def initUI(self):
        self.QUIT = Button(self, text="Quit", fg="red", command=self.quit)
        self.QUIT.place(x=50, y=5)

        self.hi_there = Button(self)
        self.hi_there["text"] = "Hello",
        self.hi_there["command"] = self.say_hi
        self.hi_there.pack({"side": "left"})

        kpdf = Image.open("../qt/kpdf.png")
        self.kpdftk = ImageTk.PhotoImage(kpdf)
        self.label1 = Label(self, image=self.kpdftk)
        self.label1.place(x=200, y=10)

        self.parent.title("Sample V.01")
        self.pack(fill=BOTH, expand=1)

root = Tk()
root.geometry("250x100+50+25")
app = App(root)
app.mainloop()
root.destroy()


