Select distinct City 
from SalesLT.Address
join SalesLT.SalesOrderHeader 
on SalesLT.SalesOrderHeader.ShipToAddressID = SalesLT.Address.AddressID
where ShipDate <= GETDATE()
order by City 