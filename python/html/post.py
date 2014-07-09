#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib, urllib2, cookielib, socket, json, base64

server_url="http://labs.simple-different.net/payment/iab_verify.php"

data={"packageName":"com.simple_different.android",
  "productId":"pro_2y",
  "purchaseToken":"bppgdhkepcijnkcignhbchbg.AO-J1Oydbejisw_ndc4zt47yU5xKEuo38kAgxOa7GdblgX7s6Ts5V_2OhaIG__ELw8m4nEnXBATswqVcUBAXX_JWIv7YvrPpSniDqN-JIT-hDsAHCCYE7KowVc5iaN8ABB8iP-u2Kk8-",
  "uid":"1082",
  "sid":"1499",
  "orderId":"12999763169054705758.1306450723735179",
  "price":u"฿1,600.00"}

print "DATA:", repr(data)

data_string = json.dumps(data)
print "JSON:", data_string

encode64 = base64.b64encode(data_string)
print "BASE64:", encode64

urlencode = urllib.urlencode({"data":encode64})
print "URLENCODE:", urlencode

# setup socket connection timeout in seconds
timeout = 15
socket.setdefaulttimeout(timeout)

# setup cookie handler
cookie_jar = cookielib.LWPCookieJar()
cookie = urllib2.HTTPCookieProcessor(cookie_jar)

# setup proxy handler, in case some-day you need to use a proxy server
proxy = {} # example: {"http" : "www.blah.com:8080"}

# create an urllib2 opener()
#opener = urllib2.build_opener(proxy, cookie) # with proxy
opener = urllib2.build_opener(cookie) # we are not going to use proxy now

#req = urllib2.Request(server_url, urlencode, header)
req = urllib2.Request(server_url, urlencode)

opener = urllib2.build_opener()
res = opener.open(req)

print "RESPONSE:", res.read()

