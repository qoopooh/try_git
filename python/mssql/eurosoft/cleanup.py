#!/usr/bin/env python
# -- coding: utf8 --

__version__ = '1.0.6.3'

HOST='192.168.1.67'
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

GET_TABLES = "SELECT * FROM information_schema.tables"
GET_COLUMNS = """SELECT column_name FROM information_schema.columns
WHERE table_name = ?"""

# Need string format
GET_ROW_COUNT = "SELECT COUNT(*) FROM {0}"
DUP = """
;WITH cte
AS (SELECT ROW_NUMBER() OVER (PARTITION BY {0}
ORDER BY ( SELECT 0)) RN
FROM   {1})
DELETE FROM cte
WHERE  RN > 1
""" # 0: column names, 1: table

NewTrans_ID = "SELECT * FROM tblNewTyreTransaction WHERE NewTrans_ID = ?"
RejectTrans_ID = "SELECT * FROM tblRejectTransaction WHERE RejectTrans_ID = ?"


def get_db():
    import pyodbc
    cnx = pyodbc.connect(CONN)
    return cnx, cnx.cursor()

def query(q, params=None):
    """query as SELECT
        if it's correct, you will got:

        rowcount, rows, len, headers
    """
    connection, cursor = get_db()
    if params is None:
        cursor.execute(q)
    elif isinstance(params, tuple) or isinstance(params, list):
        cursor.execute(q, params)
    else:
        params = (params,)
        cursor.execute(q, params)
    nr = cursor.rowcount
    if cursor.description is None:
        cursor.close()
        connection.close()
        return nr

    cols = [col[0] for col in cursor.description]
    rows = cursor.fetchall()
    cursor.close()
    connection.close()
        
    return nr, tuple(rows), len(rows), tuple(cols) 

def commit(q):
    connection, cursor = get_db()
    nr = cursor.execute(q).rowcount
    cursor.commit()
    cursor.close()
    connection.close()

def get_table_name(csv=False, catalog='Eurosoft'):
    i, rows, nr, cols = query(GET_TABLES)
    tn = [r[2] for r in rows if r[0] == catalog]
    tn.sort()
    if not csv:
        return tn

    table_names = ""
    for t in tn:
        if 'tbl' in t:
            table_names += str(t) + ", "
    return table_names[:-2]

def get_row_count(table):
    i, rows, nr, cols = query(GET_ROW_COUNT.format(table))
    return rows[0][0]

def print_row_count():
    tn = get_table_name()
    for t in tn:
        if 'tbl' in t:
            print t, get_row_count(t)

def remove_duplicated_rows(table):
    i, rows, nr, cols = query(GET_COLUMNS, table)
    col_names = " "
    for r in rows:
        col_names += str(r[0]) + ", "
    col_names = col_names[:-2] + " "
    commit(DUP.format(col_names, table))

def rem_dt():
    tn = [u'tblBrand', u'tblBuffing', u'tblBuilding', u'tblCasing', u'tblCasingStock', u'tblCasingTransaction', u'tblCasingTransactionDetail', u'tblCasingTransactionType', u'tblCementing', u'tblChamber', u'tblCompany', u'tblCompanyBranch', u'tblConfig', u'tblControl', u'tblControlFormat', u'tblCuring', u'tblCuringTemp', u'tblCustomKB', u'tblDepartment', u'tblDocument', u'tblFilling', u'tblFinalInspection', u'tblFunctions', u'tblInner', u'tblLiner', u'tblLinerReserve', u'tblLinerStock', u'tblLinerTransaction', u'tblLinerTransactionDetail', u'tblLinerTransactionType', u'tblLinerUnit', u'tblLinerUsed', u'tblMachine', u'tblMachineType', u'tblModel', u'tblModelSize', u'tblNewTyreStock', u'tblNewTyreTransaction', u'tblNewTyreTransactionDetail', u'tblNewTyreTransactionType', u'tblOuter', u'tblPreInspection', u'tblProduction', u'tblProductionDetail', u'tblProductionStatus', u'tblProductionStock', u'tblProductionTransaction', u'tblProductionTransactionDetail', u'tblProductionTransactionType', u'tblRadius', u'tblRawMat', u'tblRawMatStock', u'tblRawMatTransaction', u'tblRawMatTransactionDetail', u'tblRawMatTransactionType', u'tblRawmatType', u'tblRejectStock', u'tblRejectTransaction', u'tblRejectTransactionDetail', u'tblRejectTransactionType', u'tblRejectType', u'tblRepairing', u'tblRimming', u'tblRole', u'tblRolePrivilege', u'tblSize', u'tblSkiving', u'tblSupplier', u'tblTyre', u'tblTyreHistory', u'tblUnit', u'tblUnitConversion', u'tblUser']
    # except u'tblEmployee'
    for t in tn:
        print t, get_row_count(t)
        remove_duplicated_rows(t)
        print t, get_row_count(t)

if __name__ == '__main__':
    rem_dt()
    #print_row_count()

    print 'Done'
