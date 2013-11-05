#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, json, collections
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

OUTPUT_FILE = 'output.js'

conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa',
        database='BioStar', as_dict=True)
cur = conn.cursor()
today=1383523200
query = TIME_TODAY.format(date=today)
cur.execute(query)
rows=cur.fetchall()
#print query, rows
count = 0
rowarray_list = []
for row in rows:
    start_time = strftime("%Y:%m:%d ", localtime(today)) \
        + strftime("%H:%M:%S", localtime(row['nStartTime']))
    print row['sDepartment'], start_time, row['sUserName']
    result = row['sDepartment'], start_time, row['sUserName']
    t = []
    t.extend(result)
    rowarray_list.append(t)
    count += 1

print 'count', count
j = json.dumps(rowarray_list, ensure_ascii=False, indent=2).encode("utf8")
print 'json', j, type(j)
f = open(OUTPUT_FILE, 'w')
f.write(j)
f.close()

conn.close()

