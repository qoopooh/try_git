
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
SELECT Tyre_SerialNo,Size_Name,Model_Name
FROM tblRejectTransactionDetail,tblCasing,tblTyre,tblSize,tblModel
WHERE RejectTransDetail_Casing_ID=Casing_ID
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND RejectTransDetail_RejectTrans_ID='{tid}'
"""

INV_NT = """
SELECT Size_Name, COUNT(Size_ID)
FROM tblNewTyreStock,tblTyre,tblSize
WHERE NewStock_Tyre_Serial=Tyre_SerialNo
    AND NewStock_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
GROUP BY Size_ID
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
GROUP BY Size_ID
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
GROUP BY Size_ID
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
GROUP BY Size_ID
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
SELECT Tyre_SerialNo, Tyre_Code, Size_Name, B
, Model_Name
FROM tblTyre, tblSize, tblBrand, tblModel
WHERE Tyre_Size_ID=Size_ID
    AND Tyre_Brand_ID=Brand_ID
    AND Tyre_Model_ID=Model_ID
    AND Tyre_SerialNo='{sn}'
"""



