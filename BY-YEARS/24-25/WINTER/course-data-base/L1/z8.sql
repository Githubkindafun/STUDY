/*
Get familiar with the constraint SalesOrderHeader.CK_SalesOrderHeader_ShipDate
and show its create statement.
Try to add a (or modify existing) data row which [1]
violates the constraint. 
Show the results.
Then turn off the constraint and try again. [2]
Finally, turn on the constraint again and list the current violations. [3]

*/
-- constarint = wiêz integralnoœci (checha danej kolumny) np. : not null, identity

-- ALTER TABLE [SalesLT].[SalesOrderHeader]  WITH NOCHECK ADD  CONSTRAINT [CK_SalesOrderHeader_ShipDate] CHECK  (([ShipDate]>=[OrderDate] OR [ShipDate] IS NULL))
-- sprawdza czy data jest ok albo czy jest nullem i NOCHECK oznacza ze nie zaplikuje tego wstecznie

--ALTER TABLE SalesLT.SalesOrderHeader NOCHECK CONSTRAINT CK_SalesOrderHeader_ShipDate; -- turning off the constraint [2]
--ALTER TABLE SalesLT.SalesOrderHeader CHECK CONSTRAINT CK_SalesOrderHeader_ShipDate; -- turning on the constraint [3]

/*
INSERT INTO SalesLT.SalesOrderHeader 
( RevisionNumber, OrderDate, DueDate, ShipDate, Status, OnlineOrderFlag, 
  PurchaseOrderNumber, AccountNumber, CustomerID, ShipToAddressID, BillToAddressID, ShipMethod, 
  CreditCardApprovalCode, SubTotal, TaxAmt, Freight, Comment, rowguid, ModifiedDate )
VALUES 
( 
    2,
    '2008-06-08 00:00:00.000', 
    '2008-06-13 00:00:00.000', 
    '2008-06-01 00:00:00.000',
    5,
    0,
    'PO3770176273',
    '10-4020-000151',
    29847,
    1092,
    1092,
    'CARGO TRANSPORT 5',
    NULL,
    880.3484,
    70.4279,
    22.0087,
    NULL,
    --'89E42CDC-8506-48A2-B89B-EB3E64E3554E', -- here was an error couse that is an duplicate
	--'89E42CDC-8506-48A2-B89B-EB3E64E3554F',
	'89E42CDC-8506-48A2-B89B-EB3E64E3554A',
    '2008-06-08 00:00:00.000'
);

*/

DBCC CHECKCONSTRAINTS WITH ALL_CONSTRAINTS; -- [3]


 /*
 Msg 547, Level 16, State 0, Line 14
The INSERT statement conflicted with the CHECK constraint "CK_SalesOrderHeader_ShipDate". The conflict occurred in database "AdventureWorksLT2022", table "SalesLT.SalesOrderHeader".
The statement has been terminated
*/ -- firts error [1]

/*

Msg 2627, Level 14, State 1, Line 18
Violation of UNIQUE KEY constraint 'AK_SalesOrderHeader_rowguid'. Cannot insert duplicate key in object 'SalesLT.SalesOrderHeader'. The duplicate key value is (89e42cdc-8506-48a2-b89b-eb3e64e3554f).
The statement has been terminated.


*/ -- 2ed error but i guess that is not the point so i ll get a nondupilcate example

/*
Msg 547, Level 16, State 0, Line 18
The INSERT statement conflicted with the CHECK constraint "CK_SalesOrderHeader_ShipDate". The conflict occurred in database "AdventureWorksLT2022", table "SalesLT.SalesOrderHeader".
The statement has been terminated.
*/ -- here it is