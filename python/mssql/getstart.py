#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pyodbc
import json
import collections
import eurosoft

#cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=COMTHAI;DATABASE=EUROSOFT;UID=sa;PWD=sa')
cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=192.168.1.152;DATABASE=EUROSOFT;UID=sa;PWD=sa')
cursor = cnxn.cursor();

query = eurosoft.WIT_CUS
#query = eurosoft.WIT_NT_ID.format(id='NTO13100015')
#query = eurosoft.WIT_CUS_ID.format(id='PRI13100011')
#print query
cursor.execute(query)

rows = cursor.fetchall()

rowarray_list = []
count = 0
for r in rows:
#t = (row.Tyre_SerialNo, row.Size_Name, row.Model_Name)
    t = (r[0], r[1])
    rowarray_list.append(t)
    count += 1

j = json.dumps(rowarray_list, ensure_ascii=False, indent=2).encode("utf8")
#print j
print 'นับ', count
rowarray_file = 'output.js'
f = open(rowarray_file, 'w')
print >> f, j

cnxn.close()

