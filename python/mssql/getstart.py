#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pyodbc
import json
import collections
from eurosoft import *

#cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=COMTHAI;DATABASE=EUROSOFT;UID=sa;PWD=sa')
cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=192.168.1.153;DATABASE=EUROSOFT;UID=sa;PWD=sa')
cursor = cnxn.cursor();

#query = WIT_NT
#query = WIT_NT_ID.format(tid='NTO13100011', cid='132')
#query = WIT_NT_ID.format(tid='NTO13070031', cid='254')
#query = WIT_CUS
#query = WIT_CUS_ID.format(tid='PRI13100011', cid='124')
#query = WIT_CUS_ID.format(tid='PRO13100022', cid='95')
query = WIT_STO
#query = WIT_STO_ID.format(tid='PRI13100011', cid='8')
#query = WIT_REJ
#query = WIT_REJ_ID.format(tid='RJI13100003', cid='116')
#query = INV_NT
#query = INV_NT_ID.format(sz='295/80R22.5')
#query = INV_CUS
#query = INV_CUS_ID.format(sz='295/80R22.5')
#query = INV_STO
#query = INV_STO_ID.format(sz='295/80R22.5')
#query = INV_REJ
#query = INV_REJ_ID.format(sz='295/80R22.5')
#query = IDENTIFY.format(sn='CVO46179H')
cursor.execute(query)

rows = cursor.fetchall()

rowarray_list = []
count = 0
for r in rows:
#t = (row.Tyre_SerialNo, row.Size_Name, row.Model_Name)
    t = []
    t.extend(r)
    rowarray_list.append(t)
    count += 1

j = json.dumps(rowarray_list, ensure_ascii=False, indent=2).encode("utf8")
print j
print 'count', count
rowarray_file = 'output.js'
f = open(rowarray_file, 'w')
print >> f, j

cnxn.close()

