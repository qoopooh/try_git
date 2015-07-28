#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import json

from HTMLParser import HTMLParser


class MyHTMLParser(HTMLParser):
  __value, __sell, __buy = 0, 0, 0
  prices, __fund = [], []

  def handle_starttag(self, tag, attrs):
    pass

  def handle_endtag(self, tag):
    pass

  def handle_data(self, data):
    #u = unicode(data, 'iso-8859-11') # same as 'tis-620'
    u = data.decode('iso-8859-11')
    if u'กองทุนเปิด' in u:
      self.__fund = []
      self.__fund.append(u)
      self.__value = 3
    elif self.__value > 0:
      self.__value -= 1
      if self.__value == 0:
        self.__fund.append(data)
        self.__sell = 2
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
    """Fetch data from aia server
    """
    req = urllib2.Request(url='https://direct.aia.co.th/btob/FundQueryServlet?actionType=show',
    data='This data is passed to stdin of the CGI')
    f = urllib2.urlopen(req)
    #f = open('fund.html', 'r')
    parser = MyHTMLParser()
    parser.feed(f.read())
    self.__price = parser.prices
    return self.__price

  def string(self):
    """Show output as string
    """
    p = self.getinfo()
    sp = '::'
    out = ''
    for f in p:
      out += sp.join(f) + '\n'
      #print f[0], f[1], f[2], f[3]
    return out

  def json(self):
    """Show output as json
    """
    if len(self.__price) < 1:
        self.getinfo()
    j = json.dumps(self.__price)
    return j


def main():
  a = AiaFund()
  o = a.string()
  #o = a.json()
  print o

if __name__ == '__main__':
  main()

