#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.0.0'

HOST='192.168.1.156'
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
import web, pymssql

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
SELECT TOP 100 ProdTransDetail_ProdTrans_ID,Comp_Name,ProdTransDetail_IsConfirm,
       Comp_ID,COUNT(Comp_ID) AS cnt
FROM tblProductionTransactionDetail,tblCasing,tblCompany,tblProductionTransaction
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND Casing_OwnerBranch_ID>0
    AND ProdTransDetail_ProdTrans_ID=ProdTrans_ID
GROUP BY ProdTransDetail_ProdTrans_ID,Comp_Name,ProdTransDetail_IsConfirm,
      Comp_ID,ProdTrans_Create_Date
ORDER BY ProdTrans_Create_Date DESC
"""

#WIT_CUS_ID = """
#SELECT Tyre_SerialNo,Size_Name,Model_Name
#FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,tblModel,tblCompany
#WHERE ProdTransDetail_Casing_ID=Casing_ID
    #AND Casing_Owner_ID=Comp_ID
    #AND Casing_Tyre_Serial=Tyre_SerialNo
    #AND Casing_Tyre_Code=Tyre_Code
    #AND Tyre_Size_ID=Size_ID
    #AND Tyre_Model_ID=Model_ID
    #AND Comp_ID='{cid}'
    #AND ProdTransDetail_ProdTrans_ID='{tid}'
#"""

#WIT_CUS_ID = """
#SELECT Tyre_SerialNo,Size_Name,LinerReserve_Liner_ID
#FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,tblCompany,
     #tblProductionDetail,tblLinerReserve
#WHERE ProdTransDetail_Casing_ID=Casing_ID
    #AND Casing_Owner_ID=Comp_ID
    #AND Casing_Tyre_Serial=Tyre_SerialNo
    #AND Casing_Tyre_Code=Tyre_Code
    #AND Tyre_Size_ID=Size_ID
    #AND ProdTransDetail_Casing_ID=ProdDetail_Casing_ID
    #AND ProdDetail_Prod_ID=LinerReserve_Prod_ID
    #AND Comp_ID='{cid}'
    #AND ProdTransDetail_ProdTrans_ID='{tid}'
#"""

WIT_CUS_ID = """
SELECT Tyre_SerialNo,Size_Name,Liner_Name
FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,tblCompany,
     tblProductionDetail,tblLinerReserve,tblLiner
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND ProdTransDetail_Casing_ID=ProdDetail_Casing_ID
    AND ProdDetail_Prod_ID=LinerReserve_Prod_ID
    AND LinerReserve_Liner_ID=Liner_ID
    AND Comp_ID='{cid}'
    AND ProdTransDetail_ProdTrans_ID='{tid}'
GROUP BY Tyre_SerialNo,Size_Name,Liner_Name
"""

WIT_STO = """
SELECT TOP 100 ProdTransDetail_ProdTrans_ID,Comp_Name,ProdTransDetail_IsConfirm,
       Comp_ID,COUNT(Comp_ID) AS cnt
FROM tblProductionTransactionDetail,tblCasing,tblCompany,tblProductionTransaction
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND Casing_OwnerBranch_ID=-1
    AND ProdTransDetail_ProdTrans_ID=ProdTrans_ID
GROUP BY ProdTransDetail_ProdTrans_ID,Comp_Name,ProdTransDetail_IsConfirm,
      Comp_ID,ProdTrans_Create_Date
ORDER BY ProdTrans_Create_Date DESC
"""

WIT_STO_ID = WIT_CUS_ID

WIT_REJ = """
SELECT TOP 100 RejectTransDetail_RejectTrans_ID,Comp_Name,
       RejectTransDetail_IsConfirm,Comp_ID,COUNT(Comp_ID) as cnt
FROM tblRejectTransactionDetail,tblCasing,tblCompany,tblRejectTransaction
WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND RejectTransDetail_RejectTrans_ID=RejectTrans_ID
GROUP BY RejectTransDetail_RejectTrans_ID,Comp_Name,RejectTransDetail_IsConfirm,
      Comp_ID,RejectTrans_Create_Date
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
SELECT Size_Name,COUNT(Size_ID)
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
SELECT Size_Name,COUNT(Size_ID)
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
SELECT Size_Name,COUNT(Size_ID)
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
SELECT Size_Name,COUNT(Size_ID)
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
    conn = pymssql.connect(host=HOST, user=USER, password=PASSWORD,
        database=DATABASE, charset='utf8', as_dict=as_dict)

    cur = conn.cursor()
    cur.execute(query)
    rows = cur.fetchall()
    conn.close()
    return rows
    

class index():
    def GET(self):
        return render.idx()

class Table():

    def GET(self):
        q = self.get_query(web.input(
            action='WIT_NT', cid=None, tid=None, sn=None))
        return self.show_resp(q)

    def get_query(self, i):
        act = i['action']
        if act=='WIT_NT': q=WIT_NT
        elif act=='WIT_NT_ID': q=WIT_NT_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='WIT_CUS': q=WIT_CUS
        elif act=='WIT_CUS_ID': q=WIT_CUS_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='WIT_STO': q=WIT_STO
        elif act=='WIT_STO_ID': q=WIT_STO_ID.format(tid=i['tid'],cid=i['cid'])
        elif act=='WIT_REJ': q=WIT_REJ
        elif act=='WIT_REJ_ID': q=WIT_REJ_ID.format(tid=i['tid'],cid=i['cid'])
        else: q=IDENTIFY.format(tid=i['sn'])
        return q

    def show_resp(self, query):
        resp = ask(query, as_dict=True)
        return render.tyretable(resp)

class Json(Table):

    def GET(self):
        q = self.get_query(web.input(
            action='WIT_NT', cid=None, tid=None, sn=None))
        return self.show_resp(q)

    def show_resp(self, query):
        resp = ask(query)
        web.header('Content-Type', 'application/json')
        j = json.dumps(resp, ensure_ascii=False, indent=2)
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

