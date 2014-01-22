#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '0.1'

import sys
from time import strftime, localtime
import pymssql, web
from date import gmt

SQL_SERVER = "aaeraid\\sqlexpress"
USER = "cis"
PASSWD = USER
DATABASE = "unikwareData"

MONTHLY_REPORT = """
SELECT TB_TA_RESULT.nStartTime, TB_TA_RESULT.nEndTime, TB_USER.sUserName, TB_USER.sUserID, TB_USER_DEPT.sDepartment,TB_TA_RESULT.nDateTime
FROM TB_USER INNER JOIN
        TB_USER_DEPT ON TB_USER.nDepartmentIdn = TB_USER_DEPT.nDepartmentIdn INNER JOIN
        TB_TA_RESULT ON TB_USER.nUserIdn = TB_TA_RESULT.nUserIdn
WHERE TB_TA_RESULT.nDateTime >= {startdate}
    AND TB_TA_RESULT.nDateTime <= {enddate}
ORDER BY TB_USER_DEPT.sDepartment,CONVERT(INT, TB_USER.sUserID),TB_TA_RESULT.nDateTime
"""

OUTPUT_JS = 'output.js'

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

def gen_report(dt=0,department=None,startdate=0,enddate=0):

    count = 0
    rowarray_list = []

    conn = pymssql.connect(host=HOST, user=USER, password=PASSWORD,
            database=DATABASE, as_dict=True)
    cur = conn.cursor()
    if dt == 0:
        query = MONTHLY_REPORT.format(startdate=startdate, enddate=enddate)
    elif department == 'production':
        query = PRODUCTION_REPORT.format(date=dt)
    else:
        query = ADMIN_REPORT.format(date=dt)
    cur.execute(query)
    rows=cur.fetchall()
    conn.close()

    for row in rows:
        if dt > 0:
            date = strftime("%Y-%m-%d", localtime(dt))
        else:
            date = strftime("%Y-%m-%d", localtime(row['nDateTime']))
        start_time = strftime("%M:%S", localtime(row['nStartTime']))
        end_time = strftime("%M:%S", localtime(row['nEndTime']))
        work = strftime("%M:%S", \
                localtime(work_time(row['nStartTime'],row['nEndTime'])))
        result = row['sUserID'], date, row['sUserName'], row['sDepartment'], \
            start_time, end_time, work
        t = []
        t.extend(result)
        rowarray_list.append(t)
        count += 1

    return rowarray_list

class index:

    def GET(self):
        i = web.input(search=None,group=None)
        return render.article(i.search, i.group)

urls = (
    '/', 'index',
)

template_globals = {
    'app_path': lambda p: web.ctx.homepath + p,
}
render = web.template.render('templates/', globals=template_globals)

if __name__ == '__main__':
    web.config.debug = True
    app = web.application(urls, globals())
    app.run()

