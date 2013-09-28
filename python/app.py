#!/usr/bin/env python
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

    filemenu = wx.Menu()
    menuAbout = filemenu.Append(wx.ID_ABOUT, "&About", " Information about this program")
    filemenu.AppendSeparator()
    menuExit = filemenu.Append(wx.ID_EXIT, "E&xit", " Terminate the program")
    menuBar = wx.MenuBar()
    menuBar.Append(filemenu, "&File")
    self.SetMenuBar(menuBar)
    self.CreateStatusBar()

    self.Bind(wx.EVT_TEXT_ENTER, self.OnPressEnter, self.entry)
    self.Bind(wx.EVT_BUTTON, self.OnButtonClick, button)
    self.Bind(wx.EVT_MENU, self.OnAbout, menuAbout)
    self.Bind(wx.EVT_MENU, self.OnExit, menuExit)

    sizer.AddGrowableCol(0)
    self.SetSizerAndFit(sizer)
    self.SetSizeHints(self.GetSize().x, self.GetSize().y, -1, self.GetSize().y)
    self.Show(True)

  def OnButtonClick(self, event):
    self.label.SetLabel(self.entry.GetValue() + u' เธอกดปุ่ม')
    self.entry.SetFocus()
    self.entry.SetSelection(-1, -1)

  def OnPressEnter(self, event):
    print u'เธอเคาะ enter'

  def OnAbout(self, e):
    d = wx.MessageDialog(self, "A small app", "About simple app", wx.OK)
    d.ShowModal()
    d.Destroy()

  def OnExit(self, e):
    self.Close(True)

if __name__=="__main__":
  app = wx.App(redirect=1, filename='app.log')
  frame = simpleapp(None, -1, u'ทดสอบ')
  app.MainLoop()

