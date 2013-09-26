#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import json

from HTMLParser import HTMLParser


class MyHTMLParser(HTMLParser):
  __sell, __buy = 0, 0
  prices, __fund = [], []

  def handle_starttag(self, tag, attrs):
    #print 'start ', tag
    return
  def handle_endtag(self, tag):
    return
  def handle_data(self, data):
    u = unicode(data, 'tis-620')
    if u'กองทุนเปิด' in u:
      self.__fund = []
      self.__fund.append(u)
      self.__sell = 5
    elif self.__sell > 0:
      self.__sell -= 1
      if self.__sell == 0:
        self.__fund.append(data)
        self.__buy = 2
    elif self.__buy > 0:
      self.__buy -= 1
      if self.__buy == 0:
        self.__fund.append(data)
        self.prices.append(self.__fund)

class AiaFund():
  __price = []
  def getinfo(self):
    req = urllib2.Request(url='https://direct.aia.co.th/btob/FundQueryServlet?actionType=show',
    data='This data is passed to stdin of the CGI')
    f = urllib2.urlopen(req)
#f = open('fund.html', 'r')
    parser = MyHTMLParser()
    parser.feed(f.read())
    self.__price = parser.prices
    return self.__price
  def show(self):
    p = self.getinfo()
    for f in p:
      print f[0], f[1], f[2]

  def json(self):
    p = self.getinfo()
    j = json.dumps(p)
    return j


def main():
  a = AiaFund()
  a.show()

if __name__ == '__main__':
  main()

