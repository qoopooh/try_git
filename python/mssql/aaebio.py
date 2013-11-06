#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, json, collections
import pymssql, web
from time import strftime, localtime

from date import gmt

TIME_REPORT = """
SELECT TB_TA_RESULT.nStartTime, TB_TA_RESULT.nEndTime, TB_USER.sUserID, TB_USER.sUserName, TB_USER_DEPT.sDepartment
FROM TB_TA_RESULT INNER JOIN
    TB_USER ON TB_TA_RESULT.nUserIdn = TB_USER.nUserIdn LEFT OUTER JOIN
    TB_USER_DEPT ON TB_USER.nDepartmentIdn = TB_USER_DEPT.nDepartmentIdn
WHERE (TB_TA_RESULT.nDateTime = {date})
ORDER BY TB_USER_DEPT.sDepartment, CONVERT(INT, TB_USER.sUserID)
"""
OUTPUT_FILE = 'output.js'

def work_time(s, e):
    if e < 0:
        return 0
    if s < 480:
        s = 480
    elif 12*60 < s < 13*60:
        s = 12*60
    w = e-s
    if w < 1:
        return 0
    return w-60

def time_report(d=0):

    count = 0
    rowarray_list = []

    if d < 1:
        return rowarray_list

    query = TIME_REPORT.format(date=d)
    conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa',
            database='BioStar', as_dict=True)
    cur = conn.cursor()
    cur.execute(query)
    rows=cur.fetchall()
    conn.close()

    for row in rows:
        date = strftime("%Y-%m-%d", localtime(d))
        start_time = strftime("%M:%S", localtime(row['nStartTime']))
        end_time = strftime("%M:%S", localtime(row['nEndTime']))
        work = strftime("%M:%S", \
                localtime(work_time(row['nStartTime'],row['nEndTime'])))
        result = date, row['sUserID'], row['sUserName'], row['sDepartment'], \
            start_time, end_time, work
        t = []
        t.extend(result)
        rowarray_list.append(t)
        count += 1
    print 'count', count

    return rowarray_list

def create_json():
    report = time_report(gmt('yesterday'))
    j = json.dumps(report, ensure_ascii=False, indent=2).encode("utf8")
    f = open(OUTPUT_FILE, 'w')
    f.write(j)
    f.close()
    return j

class index:
    def GET(self):
        return create_json()

urls = (
    '/', 'index'
)

if __name__ == '__main__':
    app = web.application(urls, globals())
    app.run()

