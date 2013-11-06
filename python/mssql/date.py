#!/usr/bin/env python

from datetime import date, timedelta, datetime
from calendar import timegm
from re import compile

r = compile('\d{8}')

def gmt(d='today'):
    tm = date.today()
    if d == 'yesterday':
        tm -= timedelta(1)
    elif len(d) == 8:
        if r.match(d) is not None:
            tm = datetime.strptime(d, '%Y%m%d')
    cal = timegm(tm.timetuple())
    return cal

if __name__ == '__main__':
    i = gmt('yesterday')
    print i
    i = gmt('today')
    print i
    i = gmt('20131104')
    print i

