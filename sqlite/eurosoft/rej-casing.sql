
SELECT mx.tid, RejectTrans_Create_Date date,
  CASE WHEN
    (SELECT COUNT(*) FROM tblRejectStock WHERE RejectStock_Casing_ID=mx.casing)>0
  THEN 1 ELSE 0
  END InStock, Comp_Name
FROM 
(SELECT RejectTransDetail_RejectTrans_ID tid, MAX(RejectTransDetail_Serial) sn, RejectTransDetail_Casing_ID casing
FROM tblRejectTransactionDetail
WHERE RejectTransDetail_RejectTrans_ID LIKE 'RJO13%'
GROUP BY RejectTransDetail_RejectTrans_ID
ORDER BY tid) mx
LEFT OUTER JOIN tblCasing
ON mx.casing=Casing_ID
LEFT OUTER JOIN tblCompany
ON Casing_Owner_ID=Comp_ID
LEFT OUTER JOIN tblRejectTransaction
ON mx.tid=RejectTrans_ID

--SELECT mx.tid, Comp_Name, CASE WHEN (SELECT COUNT(*) FROM tblRejectStock WHERE RejectStock_Casing_ID=mx.casing)  > 0 THEN 1 ELSE 0 END InStock
--FROM tblCasing, tblCompany, 
--(SELECT RejectTransDetail_RejectTrans_ID tid, MAX(RejectTransDetail_Serial) sn, RejectTransDetail_Casing_ID casing
--FROM tblRejectTransactionDetail
--WHERE RejectTransDetail_RejectTrans_ID LIKE 'RJI130900%'
--GROUP BY RejectTransDetail_RejectTrans_ID
--ORDER BY tid) mx
--WHERE mx.casing=Casing_ID
  --AND Casing_Owner_ID=Comp_ID

--SELECT RejectTrans_Create_Date, RejectTrans_ID
--FROM tblRejectTransaction
--ORDER BY RejectTrans_Create_Date

--SELECT * FROM tblRejectStock

--SELECT mx.*, CASE WHEN (SELECT COUNT(*) FROM tblRejectStock WHERE RejectStock_Casing_ID=mx.casing)  > 0 THEN 1 ELSE 0 END cond1
--FROM tblCasing,
--(SELECT RejectTransDetail_RejectTrans_ID tid, MAX(RejectTransDetail_Serial) sn, RejectTransDetail_Casing_ID casing
--FROM tblRejectTransactionDetail
--WHERE RejectTransDetail_RejectTrans_ID LIKE 'RJI%'
  --AND RejectTransDetail_Serial=68
--GROUP BY RejectTransDetail_RejectTrans_ID
--ORDER BY tid) mx
--WHERE mx.casing=Casing_ID

--SELECT mx.*, CASE WHEN Casing_Owner_ID>50 THEN Casing_Owner_ID ELSE Casing_Tyre_Serial END cond1
--FROM tblCasing,
--(SELECT RejectTransDetail_RejectTrans_ID tid, MAX(RejectTransDetail_Serial) sn, RejectTransDetail_Casing_ID casing
--FROM tblRejectTransactionDetail
--WHERE RejectTransDetail_RejectTrans_ID LIKE 'RJI%'
  --AND RejectTransDetail_Serial=68
--GROUP BY RejectTransDetail_RejectTrans_ID
--ORDER BY tid) mx
--WHERE mx.casing=Casing_ID

--SELECT mx.*, Comp_Name name
--FROM tblCompany, tblCasing,
--(SELECT RejectTransDetail_RejectTrans_ID tid, MAX(RejectTransDetail_Serial) sn, RejectTransDetail_Casing_ID casing
--FROM tblRejectTransactionDetail
--WHERE RejectTransDetail_RejectTrans_ID LIKE 'RJI%'
  --AND RejectTransDetail_Serial=68
--GROUP BY RejectTransDetail_RejectTrans_ID
--ORDER BY tid) mx
--WHERE mx.casing=Casing_ID
  --AND Casing_Owner_ID=Comp_ID

--SELECT RejectTransDetail_RejectTrans_ID id, RejectTransDetail_Serial
--FROM tblRejectTransactionDetail
--WHERE RejectTransDetail_RejectTrans_ID LIKE 'RJI131%'

--SELECT RejectTransDetail_RejectTrans_ID tid, MAX(RejectTransDetail_Serial) sn, RejectTransDetail_Casing_ID casing
--FROM tblRejectTransactionDetail
--WHERE RejectTransDetail_RejectTrans_ID LIKE 'RJI%'
  --AND RejectTransDetail_Serial > 50
--GROUP BY RejectTransDetail_RejectTrans_ID
--ORDER BY tid

;

