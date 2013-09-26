#!/usr/bin/env python

import urllib2
from HTMLParser import HTMLParser

class MyHTMLParser(HTMLParser):
  def handle_starttag(self, tag, attrs):
    print 'start ', tag
  def handle_endtag(self, tag):
    print 'end ', tag
  def handle_data(self, data):
    if type(data) == 'unicode':
      print 'dat ', data.decode('utf-8')

req = urllib2.Request(url='https://direct.aia.co.th/btob/FundQueryServlet?actionType=show',
    data='This data is passed to stdin of the CGI')
f = urllib2.urlopen(req)

#print f.read()
parser = MyHTMLParser()
parser.feed(f.read())

