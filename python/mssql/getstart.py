#!/usr/bin/python

import pyodbc
import json
import collections

cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=aaebio\\bsserver;DATABASE=domotics;UID=sa;PWD=sa')
cursor = cnxn.cursor();
cursor.execute("""
    SELECT * FROM command
    """)

rows = cursor.fetchall()

rowarray_list = []
for row in rows:
  t = (row.id, row.serial)
  rowarray_list.append(t)

j = json.dumps(rowarray_list)
rowarray_file = 'command_rowarray.js'
f = open(rowarray_file, 'w')
print >> f, j

cnxn.close()

