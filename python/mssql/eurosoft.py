
S_REJ_DET = """ SELECT * FROM tblRejectTransactionDetail
                WHERE RejectTransDetail_IsConfirm = 1 """
S_PRO_DET = """ SELECT * FROM tblProductionTransactionDetail
                WHERE ProdTransDetail_IsConfirm != 1 OR 
                    ProdTransDetail_IsAuthorise != 1"""
S_PRO_DETN = """ SELECT * FROM tblProductionTransactionDetail
                WHERE ProdTransDetail_IsAuthorise != 1"""
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
WIT_CUS_ID = """
SELECT Tyre_SerialNo,Size_Name
FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,tblCompany
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Comp_ID='{cid}'
    AND ProdTransDetail_ProdTrans_ID='{tid}'
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
"""

WIT_REJ_ID = """
"""


