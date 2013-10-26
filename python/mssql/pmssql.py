#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, pymssql

S_REJ_DET = """ SELECT * FROM tblRejectTransactionDetail
                WHERE RejectTransDetail_IsConfirm = 1 """
S_PRO_DET = """ SELECT * FROM tblProductionTransactionDetail
                WHERE ProdTransDetail_IsConfirm != 1 OR 
                    ProdTransDetail_IsAuthorise != 1"""
S_PRO_DETN = """ SELECT * FROM tblProductionTransactionDetail
                WHERE ProdTransDetail_IsAuthorise != 1"""
WIT_NTO = """
SELECT TOP 100 NewTransDetail_NewTrans_ID, Comp_Name
FROM tblNewTyreTransactionDetail, tblDocument, tblCompany
WHERE NewTransDetail_IsConfirm = 1
    AND NewTransDetail_NewTrans_ID=Doc_Transaction_ID
    AND Doc_Comp_ID=Comp_ID
    """
WIT_NTO_ID = """
SELECT Tyre_SerialNo,Size_Name,Model_Name
FROM tblNewTyreTransactionDetail,tblTyre,tblSize,tblModel
WHERE NewTransDetail_Tyre_Serial=Tyre_SerialNo
    AND NewTransDetail_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND NewTransDetail_NewTrans_ID LIKE 'NTO13100016%'
    """

#conn = pymssql.connect(host='aaebio\\bsserver', user='sa', password='sa', database='domotics')
conn = pymssql.connect(host='192.168.1.152', user='sa', password='sa',
        database='EUROSOFT', charset='utf8')
cur = conn.cursor()
cur.execute(WIT_NTO)
row=cur.fetchone()
count = 0
while row:
    print row[0], row[1]
    row = cur.fetchone()
    count += 1

console_thai = 'นับ'
#sys.stdout.write(console_thai)
print console_thai, count
#print(console_thai, count)

conn.close()

