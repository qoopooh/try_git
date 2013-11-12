#!/usr/bin/env python
# -- coding: utf8 --

__version__ = '1.0.0'

HOST='192.168.1.153'
USER='sa'
PASSWORD='sa'
DATABASE='EUROSOFT'

###############################################################################
__all__ = (
        'WIT_NT', 'WIT_NT_ID',
        'WIT_CUS', 'WIT_CUS_ID',
        'WIT_STO', 'WIT_STO_ID',
        'WIT_REJ', 'WIT_REJ_ID',
        'INV_NT', 'INV_NT_ID',
        'INV_CUS', 'INV_CUS_ID',
        'INV_STO', 'INV_STO_ID',
        'INV_REJ', 'INV_REJ_ID',
        'IDENTIFY'
        )

import sys, json
import web
import pyodbc

WIT_NT = """
SELECT TOP 100 NewTransDetail_NewTrans_ID,Comp_Name,NewTransDetail_IsConfirm,
       Comp_ID,COUNT(Comp_ID) AS cnt
FROM tblNewTyreTransactionDetail,tblDocument,tblCompany
WHERE NewTransDetail_NewTrans_ID=Doc_Transaction_ID
    AND Doc_Comp_ID=Comp_ID
GROUP BY NewTransDetail_NewTrans_ID,Comp_Name,NewTransDetail_IsConfirm,Comp_ID,
      Doc_Date
ORDER BY Doc_Date DESC
"""

WIT_NT_ID = """
SELECT Tyre_SerialNo,Size_Name,Model_Name
FROM tblNewTyreTransactionDetail,tblTyre,tblSize,tblModel,tblCompany
WHERE NewTransDetail_Tyre_Serial=Tyre_SerialNo
    AND NewTransDetail_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND NewTransDetail_NewTrans_ID='{tid}'
    AND Comp_ID='{cid}'
"""

WIT_CUS = """
SELECT TOP 100 ProdTransDetail_ProdTrans_ID,Comp_Name,
    ProdTransDetail_IsConfirm,Comp_ID
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
ORDER BY ProdTrans_Create_Date DESC
"""

WIT_CUS_ID_CID = """
SELECT Tyre_SerialNo,Size_Name,Liner_Name
FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,tblCompany,
     tblProductionDetail,tblLiner
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND ProdTransDetail_Casing_ID=ProdDetail_Casing_ID
    AND ProdDetail_IntendLiner_ID=Liner_ID
    AND Comp_ID='{cid}'
    AND ProdTransDetail_ProdTrans_ID='{tid}'
"""
WIT_CUS_ID = """
SELECT Tyre_SerialNo,Size_Name,Liner_Name
FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,
     tblProductionDetail,tblLiner
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND ProdTransDetail_Casing_ID=ProdDetail_Casing_ID
    AND ProdDetail_IntendLiner_ID=Liner_ID
    AND ProdTransDetail_ProdTrans_ID='{tid}'
"""

WIT_STO = """
SELECT TOP 100 ProdTransDetail_ProdTrans_ID,Comp_Name,
    ProdTransDetail_IsConfirm,Comp_ID
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
ORDER BY ProdTrans_Create_Date DESC
"""

WIT_STO_ID = WIT_CUS_ID
WIT_STO_ID_CID = WIT_CUS_ID_CID

WIT_REJ = """
SELECT TOP 100 RejectTransDetail_RejectTrans_ID,Comp_Name,
       RejectTransDetail_IsConfirm,Comp_ID
FROM tblRejectTransactionDetail,tblCasing,tblCompany,tblRejectTransaction,
    (SELECT MAX(RejectTransDetail_Serial) as sn,RejectTransDetail_RejectTrans_ID as id
    FROM tblRejectTransactionDetail
    GROUP BY RejectTransDetail_RejectTrans_ID) r
WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND RejectTransDetail_RejectTrans_ID=RejectTrans_ID
    AND RejectTransDetail_RejectTrans_ID=r.id
    AND RejectTransDetail_Serial=r.sn
ORDER BY RejectTrans_Create_Date DESC
"""

WIT_REJ_ID = """
SELECT Tyre_SerialNo,Size_Name
FROM tblRejectTransactionDetail,tblCasing,tblTyre,tblSize
WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND RejectTransDetail_RejectTrans_ID='{tid}'
    AND Casing_Owner_ID='{cid}'
"""

INV_NT = """
SELECT Size_Name,COUNT(Size_ID) as Qty
FROM tblNewTyreStock,tblTyre,tblSize
WHERE NewStock_Tyre_Serial=Tyre_SerialNo
    AND NewStock_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
GROUP BY Size_Name
"""

INV_NT_ID = """
SELECT NewStock_Tyre_Serial,Model_Name
FROM tblNewTyreStock,tblTyre,tblSize,tblModel
WHERE NewStock_Tyre_Serial=Tyre_SerialNo
    AND NewStock_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND Size_Name='{sz}'
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
SELECT Casing_Tyre_Serial,Model_Name
FROM tblProductionStock,tblCasing,tblTyre,tblSize,tblModel
WHERE ProdStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Casing_OwnerBranch_ID>0
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND Size_Name='{sz}'
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
SELECT Casing_Tyre_Serial,Model_Name
FROM tblProductionStock,tblCasing,tblTyre,tblSize,tblModel
WHERE ProdStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Casing_OwnerBranch_ID=-1
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND Size_Name='{sz}'
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
SELECT Casing_Tyre_Serial,Model_Name
FROM tblRejectStock,tblCasing,tblTyre,tblSize,tblModel
WHERE RejectStock_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND Size_Name='{sz}'
"""

IDENTIFY = """
SELECT Tyre_SerialNo,Tyre_Code,Size_Name,Brand_Name,Model_Name
FROM tblTyre,tblSize,tblBrand,tblModel
WHERE Tyre_Size_ID=Size_ID
    AND Tyre_Brand_ID=Brand_ID
    AND Tyre_Model_ID=Model_ID
    AND Tyre_SerialNo='{sn}'
"""

def ask(query, as_dict=False):
    conn = pyodbc.connect('DRIVER={SQL Server};SERVER=' + HOST + ';DATABASE=' +
            DATABASE + ';UID=' + USER + ';PWD=' + PASSWORD)

    cur = conn.cursor()
    cur.execute(query)
    rows = cur.fetchall()
    conn.close()

    out_rows = []
    for r in rows:
        out_rows.append(tuple(r))
    return tuple(out_rows)


class index():
    def GET(self):
        return render.idx()

class Table():

    def GET(self):
        q = self.get_query(web.input(
            action='WIT_NT', cid=None, tid=None, sn=None))
        return self.show_resp(q)

    def get_query(self, i):
        self._input = i
        act = i['action']
        if act=='WIT_NT': q=WIT_NT
        elif act=='WIT_NT_ID': q=WIT_NT_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='WIT_CUS': q=WIT_CUS
        elif act=='WIT_CUS_ID':
            if i['cid'] is not None:
                q=WIT_CUS_ID_CID.format(tid=i['tid'],cid=i['cid'])
            else:
                q=WIT_CUS_ID.format(tid=i['tid'])
        elif act=='WIT_STO': q=WIT_STO
        elif act=='WIT_STO_ID': q=WIT_STO_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='WIT_REJ': q=WIT_REJ
        elif act=='WIT_REJ_ID': q=WIT_REJ_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='INV_NT': q=INV_NT
        elif act=='INV_NT_ID': q=INV_NT_ID.format(sz=i['sz'])
        elif act=='INV_CUS': q=INV_CUS
        elif act=='INV_CUS_ID': q=INV_CUS_ID.format(sz=i['sz'])
        elif act=='INV_STO': q=INV_STO
        elif act=='INV_STO_ID': q=INV_STO_ID.format(sz=i['sz'])
        elif act=='INV_REJ': q=INV_REJ
        elif act=='INV_REJ_ID': q=INV_REJ_ID.format(sz=i['sz'])
        else: q=IDENTIFY.format(sn=i['sn'])
        return q

    def show_resp(self, query):
        resp = ask(query, as_dict=True)
        for row in resp:
            for k in row.keys():
                if isinstance(k, int):
                    del row[k]
        resp = self.filter_PRI(resp)

        return render.tyretable(resp)

    def filter_PRI(self, resp):
        if self._input['action'] != 'WIT_STO' or len(resp) < 1:
            return resp
        if isinstance(resp[0], dict):
            for row in resp:
                if row['ProdTransDetail_ProdTrans_ID'][:3] == 'PRI':
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
            action='WIT_NT', cid=None, tid=None, sn=None))
        return self.show_resp(q)

    def show_resp(self, query):
        resp = ask(query)
        resp = self.filter_PRI(resp)
        web.header('Content-Type', 'application/json')
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

if __name__ == '__main__':
    app = web.application(urls, globals())
    app.run()

