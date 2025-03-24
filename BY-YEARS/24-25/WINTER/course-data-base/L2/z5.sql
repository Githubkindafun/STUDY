/*
Using the table type create a procedure:
taking a list of product identifiers and a date as input parameters: [1]
-- as a result in SalesLT.Product set DiscontinuedDate with a provided value for entries selected by identifiers,
, but only for ones with the empty field.
-- For the ones with some value, only an appropriate message should be printed.
*/


CREATE TYPE product_end_id as TABLE ( -- [1]
	product_id int,
	end_date datetime -- Right click on the table, choose 'Design table'.
);
GO


CREATE PROCEDURE product_end_date_add
	@product_ids product_end_id READONLY
AS
BEGIN

	IF EXISTS ( SELECT 42 from SalesLT.Product AS prod JOIN @product_ids AS updated_date ON prod.ProductID = updated_date.product_id WHERE prod.DiscontinuedDate IS NOT NULL)
	-- basicly if there is anything it will return 42 and if it will then if is true
	BEGIN
		THROW 54242, 'THERE IS ALREADY AN DICONTINUATION DATE IN DATA BASE', 1;
	END

	UPDATE SalesLT.Product
	SET DiscontinuedDate = updated_date.end_date
	from SalesLT.Product prod
	JOIN @product_ids updated_date ON prod.ProductID = updated_date.product_id

END;
GO






-- calling procedure
DECLARE @product_end_IDs product_end_id;

INSERT INTO @product_end_IDs (product_id, end_date) 
VALUES
	(680, CONVERT(datetime, '2002-06-02 00:00:00.000')),
	(706, CONVERT(datetime, '2002-06-02 00:00:00.000'));

EXEC product_end_date_add @product_ids = @product_end_IDs;

-- for cleanig ( for presentation )
UPDATE SalesLT.Product
SET DiscontinuedDate = NULL
WHERE ProductID = 680;

UPDATE SalesLT.Product
SET DiscontinuedDate = NULL
WHERE ProductID = 706;
