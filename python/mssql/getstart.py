#!/usr/bin/python

import pyodbc
import json
import collections

cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=COMTHAI;DATABASE=EUROSOFT;UID=sa;PWD=sa')
cursor = cnxn.cursor();
cursor.execute("""
    SELECT * FROM tblConfig
    WHERE Config_Value = 'false'
    """)

rows = cursor.fetchall()

rowarray_list = []
for row in rows:
  t = (row.Config_ID, row.Config_Name)
  rowarray_list.append(t)

j = json.dumps(rowarray_list)
rowarray_file = 'output.js'
f = open(rowarray_file, 'w')
print >> f, j

cnxn.close()

