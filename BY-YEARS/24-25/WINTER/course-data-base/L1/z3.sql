/* 3 columny, city name (table Adress),
number of customers from the city,
number of SalesPersons supporting customers from the city*/

Select City, 
COUNT(DISTINCT SalesLT.CustomerAddress.CustomerID) as customers,
COUNT(DISTINCT SalesLT.Customer.SalesPerson ) as salesman

from SalesLT.Address
--left join SalesLT.CustomerAddress on SalesLT.Address.AddressID = SalesLT.CustomerAddress.AddressID pokaze nam te z 0
--left join SalesLT.Customer on SalesLT.CustomerAddress.CustomerID = SalesLT.Customer.CustomerID
 join SalesLT.CustomerAddress on SalesLT.Address.AddressID = SalesLT.CustomerAddress.AddressID
 join SalesLT.Customer on SalesLT.CustomerAddress.CustomerID = SalesLT.Customer.CustomerID
Group by City
