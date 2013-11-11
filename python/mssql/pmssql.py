#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.0.0'

HOST='aaebio\\bsserver'
USER='sa'
PASSWORD='sa'
DATABASE='domotics'

###############################################################################
import sys, pymssql
#from eurosoft import *

conn = pymssql.connect(host=HOST, user=USER, password=PASSWORD, database=DATABASE)
#conn = pymssql.connect(host='192.168.1.153', user='sa', password='sa',
        #database='EUROSOFT', charset='utf8')
cur = conn.cursor()
#query = WIT_NT
#query = IDENTIFY.format(sn='CVO46179H')
query = 'SELECT * FROM command'

cur.execute(query)
#row=cur.fetchone()
rows=cur.fetchall()
print query, rows
count = 0
#while row:
for row in rows:
    for col in row:
        print col,
    print ''
    count += 1

console_thai = 'นับ'
#sys.stdout.write(console_thai)
print console_thai, count
#print(console_thai, count)

conn.close()

