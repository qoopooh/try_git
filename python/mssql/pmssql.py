#!/usr/bin/env python

import pymssql

#conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa', database='domotics')
conn = pymssql.connect(host='192.168.1.152', user='sa', password='sa', database='EUROSOFT')
cur = conn.cursor()
#cur.execute("""
    #SELECT * FROM command
    #""")
cur.execute("""
    SELECT * FROM tblConfig
    WHERE Config_Value = 'false'
    """)
row=cur.fetchone()
while row:
  print row[0], row[1]
  row = cur.fetchone()

conn.close()

