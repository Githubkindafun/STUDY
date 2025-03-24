-- exercise 5. : some triggers and some tables with time and at the end raport


-- Adventure db

-- StandardCost vs ListCost : first is the price from "factory" to the shop / second is from "shop" to the customer
/*
Lets say something about what i am supposed to de here. There are 2 options.
One: we do it in such manner that if there would be a change in a future we would have it
Two: we analyze all the orders and diffrent prizes of the orders and by this we have a raport of price diffrences but

This would make no sense becouse all the orders are from the same day.
So i'll go with the first one.
*/

-- think what more is needed to get that kind of report (if anything). huh?


-- here we have the table that we were asked to create
CREATE TABLE ProductPriceHistory (		
	ID int IDENTITY PRIMARY KEY, -- it was mad becouse it wasnt auto generating values and it cant be a null so had to add IDENTITY
	ProductID int,
	StandardCost DEC(15, 2),
	ListCost DEC(15, 2),
	PriceChangeDate datetime
);

DROP TABLE ProductPriceHistory;
-- now we need the trigger


CREATE TRIGGER change_register
ON SalesLT.Product
AFTER UPDATE
AS
BEGIN
	INSERT INTO ProductPriceHistory (
		ProductID,
		StandardCost,
		ListCost,
		PriceChangeDate) 
	SELECT 
		ins.ProductID,
		ins.StandardCost,
		ins.ListPrice,
		GETDATE()
	from inserted as ins
END;


-- i think that looks good, lets throw some test

UPDATE SalesLT.Product
SET StandardCost = 4.2
WHERE SalesLT.Product.ProductID = 710;

UPDATE SalesLT.Product
SET StandardCost = 8.4
WHERE SalesLT.Product.ProductID = 710;

UPDATE SalesLT.Product
SET ListPrice = 4.2
WHERE SalesLT.Product.ProductID = 710;

UPDATE SalesLT.Product
SET ListPrice = 8.4
WHERE SalesLT.Product.ProductID = 710;


SELECT * FROM ProductPriceHistory;


DROP TRIGGER IF EXISTS SalesLT.ProductPriceHistory;