--Alter Table SalesLT.Customer add CreditCardNumber int --dodawanie karty kredytowej do customer
/*
Update SalesLT.SalesOrderHeader
set CreditCardApprovalCode = 'haha'
where SalesOrderID in (Select top 3 SalesOrderID from SalesLT.SalesOrderHeader)
*/ -- ustawiamy 3 komórki (top 3) na haha

/*
Update SalesLT.Customer
set CreditCardNumber = 42 -- nie daje X bo da³em int jako credit card number mea culpa
where CustomerID in (Select CustomerID from SalesLT.SalesOrderHeader where CreditCardApprovalCode = 'haha')
*/

Select *
from SalesLT.Customer
join SalesLT.SalesOrderHeader on SalesLT.Customer.CustomerID = SalesLT.SalesOrderHeader.CustomerID
where SalesLT.Customer.CreditCardNumber = 42

