/*
Prepare a query which returns three columns:
last name and
first name of a customer (Customer) and
amount of money that the customer saved thanks to the received discounts
(SalesOrderDetail.UnitPriceDiscount).
*/
Select SalesLT.Customer.FirstName,
SalesLT.Customer.LastName,
SUM(SalesLT.SalesOrderDetail.UnitPrice * SalesLT.SalesOrderDetail.UnitPriceDiscount) as saved_money

from SalesLT.Customer

join SalesLT.SalesOrderHeader on SalesLT.Customer.CustomerID = SalesLT.SalesOrderHeader.CustomerID
join SalesLT.SalesOrderDetail on SalesLT.SalesOrderHeader.SalesOrderID = SalesLT.SalesOrderDetail.SalesOrderID

GROUP BY SalesLT.Customer.FirstName, SalesLT.Customer.LastName
Order by saved_money DESC

















