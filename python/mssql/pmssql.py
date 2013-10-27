#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, pymssql, eurosoft

#conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa', database='domotics')
conn = pymssql.connect(host='192.168.1.152', user='sa', password='sa',
        database='EUROSOFT', charset='utf8')
cur = conn.cursor()
cur.execute(eurosoft.WIT_NTO)
row=cur.fetchone()
count = 0
while row:
    print row[0], row[1]
    row = cur.fetchone()
    count += 1

console_thai = 'นับ'
#sys.stdout.write(console_thai)
print console_thai, count
#print(console_thai, count)

conn.close()

