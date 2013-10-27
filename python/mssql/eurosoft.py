
S_REJ_DET = """ SELECT * FROM tblRejectTransactionDetail
                WHERE RejectTransDetail_IsConfirm = 1 """
S_PRO_DET = """ SELECT * FROM tblProductionTransactionDetail
                WHERE ProdTransDetail_IsConfirm != 1 OR 
                    ProdTransDetail_IsAuthorise != 1"""
S_PRO_DETN = """ SELECT * FROM tblProductionTransactionDetail
                WHERE ProdTransDetail_IsAuthorise != 1"""
WIT_NT = """
SELECT TOP 100 NewTransDetail_NewTrans_ID, Comp_Name
FROM tblNewTyreTransactionDetail, tblDocument, tblCompany
WHERE NewTransDetail_IsConfirm = 1
    AND NewTransDetail_NewTrans_ID=Doc_Transaction_ID
    AND Doc_Comp_ID=Comp_ID
    """
WIT_NT_ID = """
SELECT Tyre_SerialNo,Size_Name,Model_Name
FROM tblNewTyreTransactionDetail,tblTyre,tblSize,tblModel
WHERE NewTransDetail_Tyre_Serial=Tyre_SerialNo
    AND NewTransDetail_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND NewTransDetail_NewTrans_ID = '{id}'
    """

WIT_CUS = """
SELECT ProdTransDetail_ProdTrans_ID, Comp_Name
FROM tblProductionTransactionDetail, tblCasing, tblCompany
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_Owner_ID=Comp_ID
    AND ProdTransDetail_IsConfirm != 1
    """
WIT_CUS_ID = """
SELECT Tyre_SerialNo,Size_Name,Model_Name
FROM tblProductionTransactionDetail,tblCasing,tblTyre,tblSize,tblModel
WHERE ProdTransDetail_Casing_ID=Casing_ID
    AND Casing_OwnerBranch_ID>0
    AND Casing_Tyre_Serial=Tyre_SerialNo
    AND Casing_Tyre_Code=Tyre_Code
    AND Tyre_Size_ID=Size_ID
    AND Tyre_Model_ID=Model_ID
    AND ProdTransDetail_ProdTrans_ID='{id}'
    """

