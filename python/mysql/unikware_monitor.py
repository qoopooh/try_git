#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.0.0'

import sys
from time import strftime, localtime
import web, MySQLdb

HOST="aaebio"
USER="berm"
PASSWD=USER
DB="unikware"

UNIK_RUN="""
SELECT *
FROM run
"""

def gen_report(dt=0,department=None,startdate=0,enddate=0):

    count = 0
    rowarray_list = []

    conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa',
            database='BioStar', as_dict=True)
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

class UnikMonitor:

    def GET(self):
        return render.unikware_monitor(self.get_info())

    def get_info(self):
        db = MySQLdb.connect(host=HOST, user=USER,
                passwd=PASSWD, db=DB)
        c=db.cursor()
        c.execute(UNIK_RUN)

        return c.fetchall()

urls = (
    '/', 'UnikMonitor',
)
template_globals = {
    'app_path': lambda p: web.ctx.homepath + p,
    'version': __version__,
}
render = web.template.render('templates/', globals=template_globals)

if __name__ == '__main__':
    app = web.application(urls, globals())
    app.run()

