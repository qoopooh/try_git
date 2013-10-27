#!/usr/bin/env python

import pyodbc
import json
import collections
import eurosoft

cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=COMTHAI;DATABASE=EUROSOFT;UID=sa;PWD=sa')
cursor = cnxn.cursor();
#cursor.execute("""
    #SELECT * FROM tblConfig
    #WHERE Config_Value = 'false'
    #""")
cursor.execute(eurosoft.WIT_NTO)

rows = cursor.fetchall()

rowarray_list = []
for r in rows:
#t = (row.Config_ID, row.Config_Name)
#t = (row.Tyre_SerialNo, row.Size_Name, row.Model_Name)
    t = (r[0], r[1])
    rowarray_list.append(t)

j = json.dumps(rowarray_list, ensure_ascii=False, indent=2).encode("utf8")
#print j
rowarray_file = 'output.js'
f = open(rowarray_file, 'w')
print >> f, j

cnxn.close()

