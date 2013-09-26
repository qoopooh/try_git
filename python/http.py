#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import types

from HTMLParser import HTMLParser

class MyHTMLParser(HTMLParser):
  def handle_starttag(self, tag, attrs):
#print 'start ', tag
    return
  def handle_endtag(self, tag):
#print 'end ', tag
    return
  def handle_data(self, data):
    u = unicode(data, 'tis-620')
    if u'กอง' in u:
      print 'dat ', u

#req = urllib2.Request(url='https://direct.aia.co.th/btob/FundQueryServlet?actionType=show',
#data='This data is passed to stdin of the CGI')
#f = urllib2.urlopen(req)

f = open('fund.html', 'r')

#print f.read()
parser = MyHTMLParser()
parser.feed(f.read())

