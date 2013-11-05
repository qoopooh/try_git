#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import pymssql
from time import strftime, localtime

TIME_TODAY = """
SELECT nStartTime,sUserName,sDepartment
FROM TB_TA_RESULT,TB_USER,TB_USER_DEPT
WHERE nDateTime={date}
    AND nStartTime<>0
    AND TB_TA_RESULT.nUserIdn=TB_USER.nUserIdn
    AND TB_USER.nDepartmentIdn=TB_USER_DEPT.nDepartmentIdn
ORDER BY sDepartment
"""

conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa',
        database='BioStar', as_dict=True)
cur = conn.cursor()
today=1383523200
query = TIME_TODAY.format(date=today)
cur.execute(query)
rows=cur.fetchall()
#print query, rows
count = 0
for row in rows:
    t = strftime("%Y:%m:%d ", localtime(today)) \
        + strftime("%H:%M:%S", localtime(row['nStartTime']))
    print row['sDepartment'], t, row['sUserName']
    count += 1

print 'count', count

conn.close()

