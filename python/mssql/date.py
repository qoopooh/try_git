#!/usr/bin/env python

from datetime import date, timedelta
from calendar import timegm

def gmt(d='today'):
    if d == 'yesterday':
        tm = date.today() - timedelta(1)
    else:
        tm = date.today()
    cal = timegm(tm.timetuple())
    return cal

if __name__ == '__main__':
    i = gmt('yesterday')
    print i
    i = gmt('today')
    print i

