#!/usr/bin/python
# -*- coding: utf-8 -*-

import wx

class simpleapp(wx.Frame):
  def __init__(self,parent,id,title):
    wx.Frame.__init__(self,parent,id,title)
    self.parent = parent
    self.initialize()

  def initialize(self):
    sizer = wx.GridBagSizer()
    self.entry = wx.TextCtrl(self, -1, value=u'ใส่ข้อความตรงนี้')
    sizer.Add(self.entry, (0,0), (1,1), wx.EXPAND)

    button = wx.Button(self,-1,label=u'กดฉัน')
    sizer.Add(button, (0,1))

    self.label = wx.StaticText(self,-1,label=u'สวัสดี')
    self.label.SetBackgroundColour(wx.BLUE)
    self.label.SetForegroundColour(wx.GREEN)
    sizer.Add(self.label, (1,0), (1,2), wx.EXPAND)

    sizer.AddGrowableCol(0)
    self.SetSizerAndFit(sizer)
    self.SetSizeHints(self.GetSize().x, self.GetSize().y, -1, self.GetSize().y)
    self.Show(True)

if __name__=="__main__":
  app = wx.App()
  frame = simpleapp(None, -1, u'ทดสอบ')
  app.MainLoop()

