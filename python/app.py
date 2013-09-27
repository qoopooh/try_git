#!/usr/bin/python
# -*- coding: utf-8 -*-

import wx

class simpleapp(wx.Frame):
  def __init__(self,parent,id,title):
    wx.Frame.__init__(self,parent,id,title)
    self.parent = parent
    self.initialize()

  def initialize(self):
    self.show(True)

if __name__=="__main__":
  app = wx.App()
  frame = simpleapp(None, -1, 'my application')
  app.MainLoop()

