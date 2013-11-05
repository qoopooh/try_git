#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, pymssql

TIME_TODAY = """
SELECT nDateTime,nUserIdn
FROM TB_TA_RESULT
WHERE nDateTime > 1383523200
"""

conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa', database='BioStar')
cur = conn.cursor()
query = TIME_TODAY
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
print console_thai, count

conn.close()

