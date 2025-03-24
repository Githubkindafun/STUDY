/*
• Each row of the report corresponds to a specific order expressed
by SalesOrderID, SalesOrderNumber, PurchaseOrderNumber columns.
• Next, based on SalesLT.SalesOrderDetail for each order calculate the total sum of the order 
with and without applied discounts. Hint: Look at the LineTotal column. What is its definition?
• Finally, add a column representing the amount of the whole order.
*/

Select 
SalesOrderHeader.SalesOrderID, -- 1
SalesOrderHeader.SalesOrderNumber, -- 1
SalesOrderHeader.PurchaseOrderNumber, -- 1
SUM(SalesOrderDetail.UnitPrice * SalesOrderDetail.OrderQty) as no_discount, -- 2
--suma bo moze byc kilka prodoktow w 1 id zamowienia
SUM(SalesOrderDetail.LineTotal) as discount, -- 2
SUM(SalesOrderDetail.OrderQty) as qty
from SalesLT.SalesOrderHeader


join SalesLT.SalesOrderDetail on SalesLT.SalesOrderHeader.SalesOrderID = SalesLT.SalesOrderDetail.SalesOrderID

group by 
SalesOrderHeader.SalesOrderID,
SalesOrderHeader.SalesOrderNumber,
SalesOrderHeader.PurchaseOrderNumber