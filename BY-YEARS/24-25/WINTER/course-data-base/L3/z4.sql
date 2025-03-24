-- exercise 4

CREATE TRIGGER modification_trigger
ON SalesLT.Customer
AFTER UPDATE
AS
BEGIN
    UPDATE SalesLT.Customer
	SET ModifiedDate = GETDATE()
	FROM SalesLT.Customer AS cus
	INNER JOIN inserted ins ON cus.CustomerID = ins.CustomerID;
END;

-- for the tests id should be changed to a diffrent number
UPDATE SalesLT.Customer
SET Suffix = '42'
WHERE CustomerID = 2;