#!/usr/bin/python

import pymssql

conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa', database='domotics')
cur = conn.cursor()
cur.execute("""
    SELECT * FROM command
    """)
row=cur.fetchone()
while row:
  print row[0], row[1]
  row = cur.fetchone()

conn.close()

