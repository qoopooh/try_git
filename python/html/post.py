#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib, urllib2, cookielib, socket, json, base64

server_url="http://labs.simple-different.net/payment/iab_verify.php"

data={"packageName":"com.simple_different.android",
  "productId":"pro_2y",
  "purchaseToken":"okcidnkeikoajncaehpcoeei.AO-J1OyHN4RLa3-5pvJv2sTJ5ZnO2KFXZNUmlI8OftOjAHUZvJHQ7OnsVj5dKpSfhHUo183HKaJ-9OgITlZp2zuPUrj-4paR3bUbFDDCAElnM-1mTR71wdgU02P_pnyWxrpeu3CNFARm",
  "uid":"1082",
  "sid":"1499",
  "orderId":"12999763169054705758.1349938567196656",
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

