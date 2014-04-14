
SELECT mx.tid, ProdTrans_Create_Date date,
  CASE WHEN
    (SELECT COUNT(*) FROM tblProductionStock WHERE ProdStock_Casing_ID=mx.casing)>0
  THEN 1 ELSE 0
  END InStock, Comp_Name
FROM 
(SELECT ProdTransDetail_ProdTrans_ID tid, MAX(ProdTransDetail_Serial) sn, ProdTransDetail_Casing_ID casing
FROM tblProductionTransactionDetail
WHERE ProdTransDetail_ProdTrans_ID LIKE 'PRO%'
GROUP BY ProdTransDetail_ProdTrans_ID) mx
LEFT OUTER JOIN tblCasing
ON mx.casing=Casing_ID
LEFT OUTER JOIN tblCompany
ON Casing_Owner_ID=Comp_ID
LEFT OUTER JOIN tblProductionTransaction
ON mx.tid=ProdTrans_ID
WHERE Casing_OwnerBranch_ID=-1
ORDER BY ProdTrans_Create_Date DESC
;

