#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, pymssql
from eurosoft import *

#conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa', database='domotics')
conn = pymssql.connect(host='192.168.1.153', user='sa', password='sa',
        database='EUROSOFT', charset='utf8')
cur = conn.cursor()
#query = WIT_STO
query = IDENTIFY.format(sn='CVO46179H')
cur.execute(query)
#row=cur.fetchone()
rows=cur.fetchall()
print query, rows
count = 0
#while row:
for row in rows:
    print row[0], row[1]
    row = cur.fetchone()
    count += 1

console_thai = 'นับ'
#sys.stdout.write(console_thai)
print console_thai, count
#print(console_thai, count)

conn.close()

