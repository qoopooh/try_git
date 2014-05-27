#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.0.0'

import sys
from datetime import datetime
import MySQLdb

#HOST="aae014"
HOST="192.168.1.44"
USER="oil"
PASSWD=USER
DB="et_service"
GET_NAME="""
SELECT employee_username
FROM employees
"""

#HOST="aaebio"
#HOST="192.168.1.15"
#USER="berm"
#PASSWD=USER
#DB="unikware"
#GET_RUNNING="""
#SELECT name
#FROM run
#WHERE login > logout
#"""

db = MySQLdb.connect(host=HOST, user=USER,
    passwd=PASSWD, db=DB)
c=db.cursor()
c.execute(GET_NAME)
#c.execute(GET_RUNNING)
raw_info = c.fetchall()
for r in raw_info:
    print r[0]

