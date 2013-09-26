#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import types

from HTMLParser import HTMLParser


class MyHTMLParser(HTMLParser):
  sell, buy = 0, 0
  prices, fund = [], []

  def handle_starttag(self, tag, attrs):
#print 'start ', tag
    return
  def handle_endtag(self, tag):
#print 'end ', tag
    return
  def handle_data(self, data):
    u = unicode(data, 'tis-620')
    if u'กองทุนเปิด' in u:
      self.fund = []
      self.fund.append(u)
      self.sell = 5
    elif self.sell > 0:
      self.sell -= 1
      if self.sell == 0:
        self.fund.append(data)
        self.buy = 2
    elif self.buy > 0:
      self.buy -= 1
      if self.buy == 0:
        self.fund.append(data)
        self.prices.append(self.fund)
  def pricelist(self):
    for f in self.prices:
      print f[0], f[1], f[2]


req = urllib2.Request(url='https://direct.aia.co.th/btob/FundQueryServlet?actionType=show', data='This data is passed to stdin of the CGI')
f = urllib2.urlopen(req)

#f = open('fund.html', 'r')

parser = MyHTMLParser()
parser.feed(f.read())
parser.pricelist()

