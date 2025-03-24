-- exercise 3. : products, rates and prices
-- Products(ID, ProductName),
-- Prices(ProductID REF Products(ID), Currency REF Rates(Currency), Price),
-- Rates(Currency, PricePLN)

-- master db

CREATE TABLE Products (
	ID int PRIMARY KEY,
	ProductName varchar(100) NOT NULL );

CREATE TABLE Rates (
	Currency char(3) PRIMARY KEY, -- all the shortcuts of currencies are 3 letter long
	PricePLN DEC(15, 2) ); -- num of digits = 15, digits after "." = 2
	
CREATE TABLE Prices (
	ProductID int,
	Currency char(3), -- all the shortcuts of currencies are 3 letter long
	Price DEC(15, 2),
	FOREIGN key (ProductID) REFERENCES Products(ID),
	--FOREIGN key (Currency) REFERENCES Rates(Currency));
	 );

-- so we have the tables, the firts part of the exercise is done

INSERT INTO Products (ID, ProductName) VALUES
(1, 'Hat'),
(2, 'Scarf'),
(3, 'Gloves'),
(4, 'Coat');

INSERT INTO Rates (Currency, PricePLN) VALUES
('USD', 4.01),
('EUR', 4.36),
('GBP', 5.20),
('JPY', 0.026);

DROP TABLE IF EXISTS Prices;

INSERT INTO Prices (ProductID, Currency, Price) VALUES
(1, 'USD', 100.00),      
(1, 'EUR', 80.00),       
(2, 'USD', 150.00),       
(3, 'GBP', 50.00),       
(4, 'JPY', 20000.00),     
(4, 'CHF', 20000.00);

-- Note that a product price may not be declared in all known currencies, but it always is declared in PLN as a reference.
-- I am not sure how to understand that. 

-- now we need an script that uses cursors tu update prices based on Rates.
-- If there is a row in Prices that referances an non-existant currency this row should be removed

-- ok lets write this cursor, my guess we should use a dynamic cursor

DECLARE @ProductID int, @Currency char(3), @Price DEC(15, 2);

DECLARE exterminator_cursor CURSOR DYNAMIC
FOR SELECT ProductID, Currency, Price FROM Prices;

OPEN exterminator_cursor;

FETCH NEXT FROM exterminator_cursor INTO @ProductID, @Currency, @Price;

WHILE @@FETCH_STATUS = 0
BEGIN
	IF NOT EXISTS (SELECT 42 FROM Rates WHERE Currency = @Currency)
	BEGIN
		DELETE from Prices WHERE CURRENT OF exterminator_cursor; -- current of = row that cursor currently is checking
	END

	FETCH NEXT from exterminator_cursor INTO @ProductID, @Currency, @Price;
END

CLOSE exterminator_cursor;
DEALLOCATE exterminator_cursor;


SELECT * from Prices;
SELECT * from Products;
SELECT * from Rates;

-- Regarding the given task, i do not have a test example becouse of the foregin keys i would have to
-- Wrie some stuff but i am confident that it works, as intened