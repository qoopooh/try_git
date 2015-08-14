#!/usr/bin/env python
# -- coding: utf8 --

__version__ = '1.0.6.1'

HOST='127.0.0.1'
HOST='192.168.1.66'
USER='sa'
PASSWORD='sa'
DATABASE='Eurosoft'


# os.name == 'nt'
CONN='DRIVER={SQL Server};SERVER=' + HOST + ';DATABASE=' + DATABASE \
     + ';UID=' + USER + ';PWD=' + PASSWORD
# os.name == 'posix'
CONN='DRIVER={FreeTDS};PORT=1433;SERVER=' + HOST + ';DATABASE=' + DATABASE \
     + ';UID=' + USER + ';PWD=' + PASSWORD
# Data Sources (ODBC)
CONN='DSN=eurosoftdatasource;UID=' + USER + ';PWD=' + PASSWORD

###############################################################################
# Test unixodbc for macport
# $ sudo port install freetds +mssql +odbc +universal
#
# /opt/local/etc/odbcinst.ini
#   [FreeTDS]
#   Description=FreeTDS Driver for Linux & MSSQL on Win32
#   Driver=/opt/local/lib/libtdsodbc.so
#   Setup=/opt/local/lib/libtdsodbc.so
#   UsageCount=1
#
# ~/.odbc.ini 
#   [eurosoftdatasource]
#   Description = "Some Description"
#   Driver = FreeTDS
#   Server = 192.168.1.66
#   Port = 1433
#   Database = Eurosoft
#   TDS_Version = 7.0
#
# Test unixodbc for linux
# $ sudo apt-get install freetds-dev freetds-bin unixodbc-dev tdsodbc
# $ pip install pyodbc 
# 
# /etc/odbcinst.ini 
#   [FreeTDS]
#   Description=FreeTDS Driver
#   Driver=/usr/lib/arm-linux-gnueabihf/odbc/libtdsodbc.so
#   Setup=/usr/lib/arm-linux-gnueabihf/odbc/libtdsS.so
#
# ~/.odbc.ini
#   [eurosoftdatasource]
#   Description = "Some Description"
#   Driver = FreeTDS
#   Server = 192.168.1.66
#   Port = 1433
#   Database = Eurosoft
#
# $ tsql -H 192.168.1.66 -p 1433 -U sa -P sa
# $ isql -v eurosoftdatasource sa sa
###############################################################################
__all__ = (
        'WIT_NT', 'WIT_NT_ID',
        'WIT_CUS', 'WIT_CUS_ID',
        'WIT_STO', 'WIT_STO_ID',
        'WIT_REJ', 'WIT_REJ_ID',
        'RCV_NT', 'RCV_NT_ID',
        'RCV_CUS', 'RCV_CUS_ID',
        'RCV_STO', 'RCV_STO_ID',
        'RCV_REJ', 'RCV_REJ_ID',
        'INV_NT', 'INV_NT_ID',
        'INV_CUS', 'INV_CUS_ID',
        'INV_STO', 'INV_STO_ID',
        'INV_REJ', 'INV_REJ_ID',
        'IDENTIFY', 'LOGIN',
        'CHK_NT', 'CHK_REJ',
        'CHK_STO', 'CHK_CUS',
        'CHK_NT_TYRE', 'CHK_REJ_TYRE',
        'CHK_PROD_TYRE', 'REM_NT_STOCK',
        'REM_PROD_STOCK', 'REM_REJ_STOCK'
        'ADD_NT_STOCK', 'ADD_PROD_STOCK',
        'ADD_REJ_STOCK'
        )

import sys, json
import web
import pyodbc

TRANS_NT = """
SELECT TOP 100 NewTrans_ID as tid, Comp_Name,
    CONVERT (VARCHAR, NewTrans_Create_Date, 20) as date,
    CASE WHEN
    NewTrans_Confirm_Date IS NULL
    THEN 1 ELSE 0
    END InStock
FROM tblNewTyreTransaction
LEFT OUTER JOIN tblDocument
ON NewTrans_ID=Doc_Transaction_ID
LEFT OUTER JOIN tblCompany
ON Doc_Comp_ID=Comp_ID
WHERE NewTrans_ID LIKE '%s'
ORDER BY NewTrans_Create_Date DESC
"""

WIT_NT = TRANS_NT % ('NTO%')
RCV_NT = TRANS_NT % ('NTI%')

WIT_NT_ID = """
SELECT Tyre_SerialNo,Size_Name,Model_Name,NewTransDetail_IsConfirm as isconf
FROM tblNewTyreTransactionDetail,tblTyre,tblSize,tblModel
WHERE NewTransDetail_Tyre_Serial=Tyre_SerialNo
    AND NewTransDetail_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND NewTransDetail_NewTrans_ID=?
"""

RCV_NT_ID = WIT_NT_ID

TRANS_PROD = """
SELECT TOP 100 ProdTrans_ID tid, Comp_Name,
CONVERT (VARCHAR, ProdTrans_Create_Date, 20) as date,
CASE WHEN
ProdTrans_Confirm_Date IS NULL
THEN 1 ELSE 0
END InStock
FROM tblProductionTransaction
LEFT OUTER JOIN tblProductionTransactionDetail
ON ProdTrans_ID=ProdTransDetail_ProdTrans_ID
LEFT OUTER JOIN tblCasing
ON ProdTransDetail_Casing_ID=Casing_ID
LEFT OUTER JOIN tblCompany
ON Casing_Owner_ID=Comp_ID
WHERE Casing_OwnerBranch_ID %s
AND ProdTransDetail_ProdTrans_ID LIKE '%s'
AND ProdTransDetail_Serial=1
ORDER BY ProdTrans_Create_Date DESC
"""

WIT_CUS = TRANS_PROD % ("> 0", "PRO%")
RCV_CUS = TRANS_PROD % ("> 0", "PRI%")

WIT_CUS_ID = """
SELECT Tyre_SerialNo,Size_Name,Liner_Name,ProdTransDetail_IsConfirm as isconf
FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,
     tblProductionDetail,tblLiner
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND ProdTransDetail_Casing_ID=ProdDetail_Casing_ID
    AND ProdDetail_IntendLiner_ID=Liner_ID
    AND ProdTransDetail_ProdTrans_ID=?
"""

RCV_CUS_ID = WIT_CUS_ID

WIT_STO = TRANS_PROD % ("= -1", "PRO%")

RCV_STO = """
SELECT TOP 100 ProdTrans_ID tid, Sup_Name Comp_Name,
CONVERT (VARCHAR, ProdTrans_Create_Date, 20) as date,
CASE WHEN
ProdTrans_Confirm_Date IS NULL
THEN 1 ELSE 0
END InStock
FROM tblProductionTransaction
LEFT OUTER JOIN tblProductionTransactionDetail
ON ProdTrans_ID=ProdTransDetail_ProdTrans_ID
LEFT OUTER JOIN tblCasing
ON ProdTransDetail_Casing_ID=Casing_ID
LEFT OUTER JOIN tblSupplier
ON Casing_Owner_ID=Sup_ID
WHERE Casing_OwnerBranch_ID=-1
AND ProdTransDetail_ProdTrans_ID LIKE 'PRI%'
AND ProdTransDetail_Serial=1
ORDER BY ProdTrans_Create_Date DESC
"""

WIT_STO_ID = WIT_CUS_ID
RCV_STO_ID = WIT_STO_ID

TRANS_REJ = """
SELECT TOP 100 RejectTrans_ID tid,Comp_Name,
  CONVERT (VARCHAR, RejectTrans_Create_Date, 20) as date,
  CASE WHEN
    RejectTrans_Confirm_Date IS NULL
  THEN 1 ELSE 0
  END InStock
FROM tblRejectTransaction
LEFT OUTER JOIN tblRejectTransactionDetail
ON RejectTrans_ID=RejectTransDetail_RejectTrans_ID
LEFT OUTER JOIN tblCasing
ON RejectTransDetail_Casing_ID=Casing_ID
LEFT OUTER JOIN tblCompany
ON Casing_Owner_ID=Comp_ID
WHERE RejectTransDetail_RejectTrans_ID LIKE '%s'
AND RejectTransDetail_Serial=1
ORDER BY RejectTrans_Create_Date DESC
"""

WIT_REJ = TRANS_REJ % ("RJO%")
RCV_REJ = TRANS_REJ % ("RJI%")

WIT_REJ_ID = """
SELECT Tyre_SerialNo,Size_Name,RejectTransDetail_IsConfirm as isconf
FROM tblRejectTransactionDetail,tblCasing,tblTyre,tblSize
WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND RejectTransDetail_RejectTrans_ID=?
"""

RCV_REJ_ID = WIT_REJ_ID

INV_NT = """
SELECT Size_Name,COUNT(Size_ID) as Qty
FROM tblNewTyreStock,tblTyre,tblSize
WHERE NewStock_Tyre_Serial=Tyre_SerialNo
    AND NewStock_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
GROUP BY Size_Name
"""

INV_NT_ID = """
SELECT Tyre_SerialNo,Model_Name
FROM tblNewTyreStock,tblTyre,tblSize,tblModel
WHERE NewStock_Tyre_Serial=Tyre_SerialNo
    AND NewStock_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND Size_Name=?
"""

INV_CUS = """
SELECT Size_Name,COUNT(Size_ID) as Qty
FROM tblProductionStock,tblCasing,tblTyre,tblSize
WHERE ProdStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Casing_OwnerBranch_ID>0
    AND Tyre_Size_ID=Size_ID
GROUP BY Size_Name
"""

INV_CUS_ID = """
SELECT Tyre_SerialNo,Model_Name
FROM tblProductionStock,tblCasing,tblTyre,tblSize,tblModel
WHERE ProdStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Casing_OwnerBranch_ID>0
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND Size_Name=?
"""

INV_STO = """
SELECT Size_Name,COUNT(Size_ID) as Qty
FROM tblProductionStock,tblCasing,tblTyre,tblSize
WHERE ProdStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Casing_OwnerBranch_ID=-1
    AND Tyre_Size_ID=Size_ID
GROUP BY Size_Name
"""

INV_STO_ID = """
SELECT Tyre_SerialNo,Model_Name
FROM tblProductionStock,tblCasing,tblTyre,tblSize,tblModel
WHERE ProdStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Casing_OwnerBranch_ID=-1
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND Size_Name=?
"""

INV_REJ = """
SELECT Size_Name,COUNT(Size_ID) as Qty
FROM tblRejectStock,tblCasing,tblTyre,tblSize
WHERE RejectStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
GROUP BY Size_Name
"""

INV_REJ_ID = """
SELECT Tyre_SerialNo,Model_Name
FROM tblRejectStock,tblCasing,tblTyre,tblSize,tblModel
WHERE RejectStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND Size_Name=?
"""

IDENTIFY = """
SELECT Tyre_SerialNo,Tyre_Code,Size_Name,Brand_Name,Model_Name
FROM tblTyre,tblSize,tblBrand,tblModel
WHERE Tyre_Size_ID=Size_ID
    AND Tyre_Brand_ID=Brand_ID
    AND Tyre_Model_ID=Model_ID
    AND Tyre_SerialNo=?
"""

LOGIN = """
SELECT Usr_Emp_ID
FROM tblUser
WHERE Usr_UserName=?
    AND Usr_Password=?
"""

CHK_NT_NULL = """
UPDATE tblNewTyreTransaction
SET NewTrans_Confirm_By=NULL,
    NewTrans_Confirm_Date=NULL
WHERE NewTrans_ID=?
"""

CHK_NT = """
UPDATE tblNewTyreTransaction
SET NewTrans_Confirm_By=?,
    NewTrans_Confirm_Date=?
WHERE NewTrans_ID=?
"""

CHK_CUS_NULL = """
UPDATE tblProductionTransaction
SET ProdTrans_Confirm_By=NULL,
    ProdTrans_Confirm_Date=NULL
WHERE ProdTrans_ID=?
"""

CHK_CUS = """
UPDATE tblProductionTransaction
SET ProdTrans_Confirm_By=?,
    ProdTrans_Confirm_Date=?
WHERE ProdTrans_ID=?
"""

CHK_REJ_NULL = """
UPDATE tblRejectTransaction
SET RejectTrans_Confirm_By=NULL,
    RejectTrans_Confirm_Date=NULL
WHERE RejectTrans_ID=?
"""

CHK_REJ = """
UPDATE tblRejectTransaction
SET RejectTrans_Confirm_By=?,
    RejectTrans_Confirm_Date=?
WHERE RejectTrans_ID=?
"""

CHK_STO_NULL = CHK_CUS_NULL
CHK_STO = CHK_CUS

CHK_NT_TYRE = """
UPDATE tblNewTyreTransactionDetail
SET NewTransDetail_IsAuthorise=1,
    NewTransDetail_IsConfirm=?
WHERE NewTransDetail_NewTrans_ID=?
    AND NewTransDetail_Tyre_Serial=?
"""

CHK_PROD_TYRE = """
UPDATE tblProductionTransactionDetail
SET ProdTransDetail_IsAuthorise=1,
    ProdTransDetail_IsConfirm=?
FROM tblProductionTransactionDetail, tblCasing
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND ProdTransDetail_ProdTrans_ID=?
    AND Casing_Tyre_Serial=?
"""

CHK_REJ_TYRE = """
UPDATE tblRejectTransactionDetail
SET RejectTransDetail_IsAuthorise=1,
    RejectTransDetail_IsConfirm=?
FROM tblRejectTransactionDetail, tblCasing
WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND RejectTransDetail_RejectTrans_ID=?
    AND Casing_Tyre_Serial=?
"""

REM_NT_STOCK = """
DELETE FROM tblNewTyreStock
WHERE EXISTS (
    SELECT 1
    FROM tblNewTyreTransaction
    WHERE NewTrans_Approve_Date IS NOT NULL
    AND NewTrans_ID=?)
AND NewStock_Tyre_Serial IN (
    SELECT NewTransDetail_Tyre_Serial
    FROM tblNewTyreTransactionDetail
    WHERE NewTransDetail_NewTrans_ID=?)
"""

REM_PROD_STOCK = """
DELETE FROM tblProductionStock
WHERE EXISTS (
    SELECT 1
    FROM tblProductionTransaction
    WHERE ProdTrans_Approve_Date IS NOT NULL
        AND ProdTrans_ID=?)
AND ProdStock_Casing_ID IN(
    SELECT ProdTransDetail_Casing_ID
    FROM tblProductionTransactionDetail
    WHERE ProdTransDetail_ProdTrans_ID=?)
"""

REM_REJ_STOCK = """
DELETE FROM tblRejectStock
WHERE EXISTS (
    SELECT 1
    FROM tblRejectTransaction
    WHERE RejectTrans_Approve_Date IS NOT NULL
        AND RejectTrans_ID=?)
AND RejectStock_Casing_ID IN(
    SELECT RejectTransDetail_Casing_ID
    FROM tblRejectTransactionDetail
    WHERE RejectTransDetail_RejectTrans_ID=?)
"""

ADD_NT_STOCK = """
INSERT tblNewTyreStock (NewStock_Tyre_Serial,NewStock_Tyre_Code)
SELECT NewTransDetail_Tyre_Serial,NewTransDetail_Tyre_Code
FROM tblNewTyreTransactionDetail
WHERE NewTransDetail_NewTrans_ID=?
AND NOT EXISTS (SELECT 1
    FROM tblNewTyreTransactionDetail,tblNewTyreStock
    WHERE NewStock_Tyre_Serial=NewTransDetail_Tyre_Serial
    AND NewStock_Tyre_Code=NewTransDetail_Tyre_Code
    AND NewTransDetail_Serial=1
    AND NewTransDetail_NewTrans_ID=?);
UPDATE tblNewTyreTransaction
SET NewTrans_Confirm_Date=NULL
WHERE NewTrans_ID=?
"""

ADD_PROD_STOCK = """
INSERT tblProductionStock (ProdStock_Casing_ID)
SELECT ProdTransDetail_Casing_ID
FROM tblProductionTransactionDetail,tblCasing
WHERE ProdTransDetail_Casing_ID=Casing_ID
AND ProdTransDetail_ProdTrans_ID=?
AND NOT EXISTS (SELECT 1
    FROM tblProductionTransactionDetail,tblCasing,tblProductionStock
    WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND ProdTransDetail_Casing_ID=ProdStock_Casing_ID
    AND ProdTransDetail_Serial=1
    AND ProdTransDetail_ProdTrans_ID=?)
"""

ADD_REJ_STOCK = """
INSERT tblRejectStock (RejectStock_Casing_ID)
SELECT Casing_ID
FROM tblCasing,tblRejectTransactionDetail
WHERE RejectTransDetail_Casing_ID=Casing_ID
AND RejectTransDetail_RejectTrans_ID=?
AND NOT EXISTS (SELECT 1
    FROM tblRejectStock,tblCasing,tblRejectTransactionDetail
    WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND RejectTransDetail_Casing_ID=RejectStock_Casing_ID
    AND RejectTransDetail_Serial=1
    AND RejectTransDetail_RejectTrans_ID=?)
"""

def ask(q):
    conn = pyodbc.connect(CONN)
    cur = conn.cursor()
    if q[1] == None:
        param = ()
    else:
        param = q[1]
    count = cur.execute(q[0], param).rowcount
    print q[0], param, count
    if cur.description == None:
        res = 'failed'
        if count > 0: # To support insert multirow
            conn.commit()
            res = 'success'
        cur.close()
        conn.close()
        return (res,)
    rows = cur.fetchall()
    cols = [col[0] for col in cur.description]
    cur.close()
    conn.close()

    out_rows = []
    for row in rows:
        rr = decode(row)
        out_rows.append(rr)
    return tuple(out_rows), tuple(cols)

def ask_json(q):
    conn = pyodbc.connect(CONN)
    cur = conn.cursor()
    if q[1] == None:
        param = ()
    else:
        param = q[1]
    count = cur.execute(q[0], param).rowcount
    print 'q', q
    print 'count', count
    print 'description', cur.description
    if cur.description == None:
        res = 'failed'
        if count > 0: # To support insert multirow
            conn.commit()
            res = 'success'
        cur.close()
        conn.close()
        return (res,)
    rows = cur.fetchall()
    cols = [col[0] for col in cur.description]
    cur.close()
    conn.close()

    out_rows = []
    for row in rows:
        rr = decode(row)
        out_rows.append(dict(zip(cols, rr)))
    return tuple(out_rows)

def decode(row):
    """Decode thai string to UTF-8
    """
    out = []
    for c in row:
        if isinstance(c, str):
            out.append(unicode(c, 'tis-620'))
        else:
            out.append(c)
    return tuple(out)

class index():
    """Index Page
    """

    def GET(self):
        inp = web.input(usr=None, passwd=None)
        if inp['usr'] == None:
            return render.idx(__version__, 
                    socket.gethostname() + ' (' + socket.gethostbyname(socket.gethostname()) + ')',
                    HOST)
        q = LOGIN
        param = (inp['usr'], inp['passwd'])
        return self.show_resp((q, param))

    def show_resp(self, query):
        resp = ask_json(query)
        web.header('Content-Type', 'application/json;charset=utf8')
        j = json.dumps(resp, ensure_ascii=False, indent=2).encode('utf8')
        return j

class Table():

    def GET(self):
        q = self.get_query(web.input(
            action='WIT_NT', tid=None, sn=None, check=None))
        web.header('Content-Type', 'text/html;charset=utf8')
        return self.show_resp(q)

    def get_query(self, i):
        self._input = i
        act = i['action']
        param = ()
        if act=='WIT_NT': q=WIT_NT
        elif act=='WIT_NT_ID': q, param = WIT_NT_ID, (i['tid'])
        elif act=='WIT_CUS': q=WIT_CUS
        elif act=='WIT_CUS_ID': q, param = WIT_CUS_ID, (i['tid'])
        elif act=='WIT_STO': q=WIT_STO
        elif act=='WIT_STO_ID': q, param = WIT_STO_ID, (i['tid'])
        elif act=='WIT_REJ': q=WIT_REJ
        elif act=='WIT_REJ_ID': q, param = WIT_REJ_ID, (i['tid'])
        elif act=='RCV_NT': q=RCV_NT
        elif act=='RCV_NT_ID': q, param = RCV_NT_ID, (i['tid'])
        elif act=='RCV_CUS': q=RCV_CUS
        elif act=='RCV_CUS_ID': q, param = RCV_CUS_ID, (i['tid'])
        elif act=='RCV_STO': q=RCV_STO
        elif act=='RCV_STO_ID': q, param = RCV_STO_ID, (i['tid'])
        elif act=='RCV_REJ': q=RCV_REJ
        elif act=='RCV_REJ_ID': q, param = RCV_REJ_ID, (i['tid'])
        elif act=='INV_NT': q=INV_NT
        elif act=='INV_NT_ID': q, param = INV_NT_ID, (i['sz'])
        elif act=='INV_CUS': q=INV_CUS
        elif act=='INV_CUS_ID': q, param = INV_CUS_ID, (i['sz'])
        elif act=='INV_STO': q=INV_STO
        elif act=='INV_STO_ID': q, param = INV_STO_ID, (i['sz'])
        elif act=='INV_REJ': q=INV_REJ
        elif act=='INV_REJ_ID': q, param = INV_REJ_ID, (i['sz'])
        elif act=='CHK_NT':
            if i['date'] == 'null':
                q, param = CHK_NT_NULL, (i['tid'])
            else:
                q, param = CHK_NT, (i['eid'],i['date'],i['tid'])
        elif act=='CHK_CUS':
            if i['date'] == 'null':
                q, param = CHK_CUS_NULL, (i['tid'])
            else:
                q, param = CHK_CUS, (i['eid'],i['date'],i['tid'])
        elif act=='CHK_REJ':
            if i['date'] == 'null':
                q, param = CHK_REJ_NULL, (i['tid'])
            else:
                q, param = CHK_REJ, (i['eid'],i['date'],i['tid'])
        elif act=='CHK_STO':
            if i['date'] == 'null':
                q, param = CHK_STO_NULL, (i['tid'])
            else:
                q, param = CHK_STO, (i['eid'],i['date'],i['tid'])
        elif act=='CHK_NT_TYRE': q, param = CHK_NT_TYRE, (i['check'],i['tid'],i['sn'])
        elif act=='CHK_PROD_TYRE': q, param = CHK_PROD_TYRE, (i['check'],i['tid'],i['sn'])
        elif act=='CHK_REJ_TYRE': q, param = CHK_REJ_TYRE, (i['check'],i['tid'],i['sn'])
        elif act=='ADD_NT_STOCK': q, param = ADD_NT_STOCK, (i['tid'],i['tid'],i['tid'])
        elif act=='ADD_PROD_STOCK': q, param = ADD_PROD_STOCK, (i['tid'],i['tid'])
        elif act=='ADD_REJ_STOCK': q, param = ADD_REJ_STOCK, (i['tid'],i['tid'])
        elif act=='REM_NT_STOCK': q, param = REM_NT_STOCK, (i['tid'],i['tid'])
        elif act=='REM_PROD_STOCK': q, param = REM_PROD_STOCK, (i['tid'],i['tid'])
        elif act=='REM_REJ_STOCK': q, param = REM_REJ_STOCK, (i['tid'],i['tid'])
        else: q, param = IDENTIFY, (i['sn'])
        return q, param

    def show_resp(self, query):
        rows, cols = ask(query)
        resp = self.filter_PRI(rows)

        return render.tyretable(resp, cols)

    def filter_PRI(self, resp):
        if self._input['action'] != 'WIT_STO' or len(resp) < 1:
            return resp
        if isinstance(resp[0], dict):
            for row in resp:
                if row['tid'][:3] == 'PRI':
                    row['Comp_Name'] = None
                    row['Comp_ID'] = 0
        else:
            l, i, tmp, resp = len(resp), 0, resp, []
            while i<l:
                if tmp[i][0][:3] == 'PRI':
                    resp.append((tmp[i][0], None, tmp[i][2], 0))
                else:
                    resp.append(tmp[i])
                i += 1

        return resp

class Json(Table):

    def GET(self):
        q = self.get_query(web.input(
            action='WIT_NT', tid=None, sn=None, check=None))
        return self.show_resp(q)

    def show_resp(self, query):
        resp = ask_json(query)
        resp = self.filter_PRI(resp)
        web.header('Content-Type', 'application/json;charset=utf8')
        j = json.dumps(resp, ensure_ascii=False, indent=2).encode('utf8')
        with open('output.js', 'w') as f:
            print >> f, j
        return j

urls = (
    '/', 'index',
    '/table/', 'Table',
    '/json/', 'Json',
)
template_globals = {
    'app_path': lambda p: web.ctx.homepath + p,
}
render = web.template.render('templates/', globals=template_globals)

import socket
if __name__ == '__main__':
    print "Eurosoft Web Application", __version__
    print "Web Host:", socket.gethostbyname(socket.gethostname())

    web.config.debug = True # comment out when release
    app = web.application(urls, globals())
    app.run()

