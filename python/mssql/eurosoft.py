#!/usr/bin/env python
# -- coding: utf8 --

__version__ = '1.0.5'

#HOST='127.0.0.1'
HOST='192.168.0.62'
USER='sa'
PASSWORD='sa'
DATABASE='EUROSOFT'

CONN='DRIVER={SQL Server};SERVER=' + HOST + ';DATABASE=' + DATABASE + ';UID=' + \
    USER + ';PWD=' + PASSWORD
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
        'CHK_PROD_TYRE',
        )

import sys, json
import web
import pyodbc

WIT_NT = """
SELECT TOP 100 NewTransDetail_NewTrans_ID as tid,Comp_Name,NewTransDetail_IsConfirm as isconf,
       Comp_ID,COUNT(Comp_ID) AS cnt
FROM tblNewTyreTransactionDetail,tblDocument,tblCompany
WHERE NewTransDetail_NewTrans_ID=Doc_Transaction_ID
    AND Doc_Comp_ID=Comp_ID
    AND NewTransDetail_NewTrans_ID LIKE '%NTO%'
GROUP BY NewTransDetail_NewTrans_ID,Comp_Name,NewTransDetail_IsConfirm,Comp_ID,
      Doc_Date
ORDER BY Doc_Date DESC
"""

RCV_NT = """
SELECT TOP 100 NewTransDetail_NewTrans_ID as tid,
       COUNT(NewTransDetail_Tyre_Serial) AS cnt
FROM tblNewTyreTransactionDetail
WHERE NewTransDetail_NewTrans_ID LIKE '%NTI%'
GROUP BY NewTransDetail_NewTrans_ID
ORDER BY NewTransDetail_NewTrans_ID DESC
"""

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

WIT_CUS = """
SELECT TOP 100 ProdTransDetail_ProdTrans_ID as tid,Comp_Name,
    ProdTransDetail_IsConfirm as isconf,Comp_ID
FROM tblProductionTransactionDetail,tblCasing,tblCompany,tblProductionTransaction,
    (SELECT MAX(ProdTransDetail_Serial) as sn,ProdTransDetail_ProdTrans_ID as id
    FROM tblProductionTransactionDetail
    GROUP BY ProdTransDetail_ProdTrans_ID) p
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND ProdTransDetail_ProdTrans_ID=ProdTrans_ID
    AND ProdTransDetail_ProdTrans_ID=p.id
    AND ProdTransDetail_Serial=p.sn
    AND Casing_OwnerBranch_ID>0
    AND ProdTransDetail_ProdTrans_ID LIKE '%PRO%'
ORDER BY ProdTrans_Create_Date DESC
"""

RCV_CUS = """
SELECT TOP 100 ProdTransDetail_ProdTrans_ID as tid,Comp_Name,
    ProdTransDetail_IsConfirm as isconf,Comp_ID
FROM tblProductionTransactionDetail,tblCasing,tblCompany,tblProductionTransaction,
    (SELECT MAX(ProdTransDetail_Serial) as sn,ProdTransDetail_ProdTrans_ID as id
    FROM tblProductionTransactionDetail
    GROUP BY ProdTransDetail_ProdTrans_ID) p
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND ProdTransDetail_ProdTrans_ID=ProdTrans_ID
    AND ProdTransDetail_ProdTrans_ID=p.id
    AND ProdTransDetail_Serial=p.sn
    AND Casing_OwnerBranch_ID>0
    AND ProdTransDetail_ProdTrans_ID LIKE '%PRI%'
ORDER BY ProdTrans_Create_Date DESC
"""

WIT_CUS_ID_CID = """
SELECT Tyre_SerialNo,Size_Name,Liner_Name
FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,tblCompany,
     tblProductionDetail,tblLiner
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code AND Tyre_Size_ID=Size_ID AND ProdTransDetail_Casing_ID=ProdDetail_Casing_ID
    AND ProdDetail_IntendLiner_ID=Liner_ID AND Comp_ID=?
    AND ProdTransDetail_ProdTrans_ID=?
"""
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

RCV_CUS_ID_CID = WIT_CUS_ID_CID
RCV_CUS_ID = WIT_CUS_ID

WIT_STO = """
SELECT TOP 100 ProdTransDetail_ProdTrans_ID as tid,Comp_Name,
    ProdTransDetail_IsConfirm as isconf,Comp_ID
FROM tblProductionTransactionDetail,tblCasing,tblCompany,tblProductionTransaction,
    (SELECT MAX(ProdTransDetail_Serial) as sn,ProdTransDetail_ProdTrans_ID as id
    FROM tblProductionTransactionDetail
    GROUP BY ProdTransDetail_ProdTrans_ID) p
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND ProdTransDetail_ProdTrans_ID=ProdTrans_ID
    AND ProdTransDetail_ProdTrans_ID=p.id
    AND ProdTransDetail_Serial=p.sn
    AND Casing_OwnerBranch_ID=-1
    AND ProdTransDetail_ProdTrans_ID LIKE '%PRO%'
ORDER BY ProdTrans_Create_Date DESC
"""

RCV_STO = """
SELECT TOP 100 ProdTransDetail_ProdTrans_ID as tid,Sup_Name as Comp_Name,
    ProdTransDetail_IsConfirm as isconf,Sup_ID as Comp_ID
FROM tblProductionTransactionDetail,tblCasing,tblSupplier,tblProductionTransaction,
    (SELECT MAX(ProdTransDetail_Serial) as sn,ProdTransDetail_ProdTrans_ID as id
    FROM tblProductionTransactionDetail
    GROUP BY ProdTransDetail_ProdTrans_ID) p
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Sup_ID
    AND ProdTransDetail_ProdTrans_ID=ProdTrans_ID
    AND ProdTransDetail_ProdTrans_ID=p.id
    AND ProdTransDetail_Serial=p.sn
    AND Casing_OwnerBranch_ID=-1
    AND ProdTransDetail_ProdTrans_ID LIKE '%PRI%'
ORDER BY ProdTrans_Create_Date DESC
"""

WIT_STO_ID = WIT_CUS_ID
RCV_STO_ID = WIT_STO_ID

WIT_REJ = """
SELECT TOP 100 RejectTransDetail_RejectTrans_ID as tid,Comp_Name,
       RejectTransDetail_IsConfirm as isconf,Comp_ID
FROM tblRejectTransactionDetail,tblCasing,tblCompany,tblRejectTransaction,
    (SELECT MAX(RejectTransDetail_Serial) as sn,RejectTransDetail_RejectTrans_ID as id
    FROM tblRejectTransactionDetail
    GROUP BY RejectTransDetail_RejectTrans_ID) r
WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND RejectTransDetail_RejectTrans_ID=RejectTrans_ID
    AND RejectTransDetail_RejectTrans_ID=r.id
    AND RejectTransDetail_Serial=r.sn
    AND RejectTransDetail_RejectTrans_ID LIKE '%RJO%'
ORDER BY RejectTrans_Create_Date DESC
"""

RCV_REJ = """
SELECT TOP 100 RejectTransDetail_RejectTrans_ID as tid,Comp_Name,
       RejectTransDetail_IsConfirm as isconf,Comp_ID
FROM tblRejectTransactionDetail,tblCasing,tblCompany,tblRejectTransaction,
    (SELECT MAX(RejectTransDetail_Serial) as sn,RejectTransDetail_RejectTrans_ID as id
    FROM tblRejectTransactionDetail
    GROUP BY RejectTransDetail_RejectTrans_ID) r
WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND RejectTransDetail_RejectTrans_ID=RejectTrans_ID
    AND RejectTransDetail_RejectTrans_ID=r.id
    AND RejectTransDetail_Serial=r.sn
    AND RejectTransDetail_RejectTrans_ID LIKE '%RJI%'
ORDER BY RejectTrans_Create_Date DESC
"""

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

CHK_NT = """
UPDATE tblNewTyreTransaction
SET NewTrans_Confirm_By=?,
    NewTrans_Confirm_Date=?
WHERE NewTrans_ID=?
"""

CHK_CUS = """
UPDATE tblProductionTransaction
SET ProdTrans_Confirm_By=?,
    ProdTrans_Confirm_Date=?
WHERE ProdTrans_ID=?
"""

CHK_REJ = """
UPDATE tblRejectTransaction
SET RejectTrans_Confirm_By=?,
    RejectTrans_Confirm_Date=?
WHERE RejectTrans_ID=?
"""

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

def ask(q):
    conn = pyodbc.connect(CONN)
    cur = conn.cursor()
    if q[1] == None:
        param = ()
    else:
        param = q[1]
    count = cur.execute(q[0], param).rowcount
    if cur.description == None:
        res = 'failed'
        if count == 1:
            conn.commit()
            res = 'success'
        conn.close()
        return (res,)
    rows = cur.fetchall()
    conn.close()

    out_rows = []
    for r in rows:
        out_rows.append(tuple(r))
    return tuple(out_rows)

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
        if count == 1:
            conn.commit()
            res = 'success'
        conn.close()
        return (res,)
    cols = [col[0] for col in cur.description]
    rows = cur.fetchall()
    conn.close()

    out_rows = []
    for row in rows:
        out_rows.append(dict(zip(cols, row)))
    return tuple(out_rows)

class index():
    def GET(self):
        inp = web.input(usr=None, passwd=None)
        if inp['usr'] == None:
            return render.idx()
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
            #if i['cid'] is not None:
                #q=WIT_CUS_ID_CID.format(tid=i['tid'],cid=i['cid'])
            #else:
                #q=WIT_CUS_ID.format(tid=i['tid'])
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
        elif act=='CHK_NT': q, param = CHK_NT, (i['eid'],i['date'],i['tid'])
        elif act=='CHK_CUS': q, param = CHK_CUS, (i['eid'],i['date'],i['tid'])
        elif act=='CHK_REJ': q, param = CHK_REJ, (i['eid'],i['date'],i['tid'])
        elif act=='CHK_STO': q, param = CHK_STO, (i['eid'],i['date'],i['tid'])
        elif act=='CHK_NT_TYRE': q, param = CHK_NT_TYRE, (i['check'],i['tid'],i['sn'])
        elif act=='CHK_PROD_TYRE': q, param = CHK_PROD_TYRE, (i['check'],i['tid'],i['sn'])
        elif act=='CHK_REJ_TYRE': q, param = CHK_REJ_TYRE, (i['check'],i['tid'],i['sn'])
        else: q, param = IDENTIFY, (i['sn'])
        return q, param

    def show_resp(self, query):
        resp = ask(query)
        resp = self.filter_PRI(resp)

        return render.tyretable(resp)

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
    app = web.application(urls, globals())
    app.run()

