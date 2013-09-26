#!/usr/bin/env python

import urllib2

req = urllib2.Request(url='https://direct.aia.co.th/btob/FundQueryServlet?actionType=show',
    data='This data is passed to stdin of the CGI')
f = urllib2.urlopen(req)

print f.read()

