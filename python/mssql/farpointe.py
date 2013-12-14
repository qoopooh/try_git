#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.0.2'

import sys
from time import strftime, localtime
from date import gmt
import pymssql
from flask import Flask


SQL_NONCOMPLIANT = """
SELECT Number, TxPower, TxModDepth, WrID, wr_name, DateTime
FROM Logger, Workorder
WHERE WrID = wr_id
AND TxPower >= 88
AND DateTime > '2013-12-01 16:30:00.000'
AND Number!='6E310104'
ORDER BY [DateTime] DESC
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

class index:

    def GET(self):
        i = web.input(name=None)
        return render.index(i.name)

class TimeReport:

    def GET(self):
        i = web.input(date=None, department='production')
        return self.create_table(i.date, i.department)

    def create_table(self, date, department):
        report = gen_report(dt=gmt(date), department=department)
        return render.time_report(report)


class MonthlyReport:

    def GET(self):
        i = web.input(startdate=None, enddate=None)
        return self.create_table(i.startdate, i.enddate)

    def create_table(self, startdate, enddate):
        report = gen_report(startdate=gmt(startdate), enddate=gmt(enddate))
        return render.time_report(report)

app = Flask(__name__)

@app.route('/')
def main():
    return 'Farpointe'

if __name__ == '__main__':
    app.run(host='0.0.0.0')

