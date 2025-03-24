-- z3
-- Create three examples showing the anomalies of 
-- dirty read, [1]
-- non-repeatable read, [2]
-- and phantom read, respectively. [3]

-- okie let's do it

/*
Dirty reads (uncommitted dependency) [1]
 Pierwsza osoba edytuje dane 
 W miêdzyczasie druga pobiera te czêœciowo zmienione 
 Nastêpnie pierwsza osoba decyduje o wycofaniu zmian
*/

/*
Niepowtarzalnoœæ odczytu (nonrepeatable read) [2]
 Aka. Inconsistent analysis. Podobne do „brudne dane” 
 Czytamy dane, ktoœ je modyfikuje, czytamy dane powtórnie 
 Wyniki obliczeñ wykonane za pierwszym i drugim razem mog¹ 
byæ inne co prowadzi do niespójnoœci
*/

/*
Odczyty fantomów (phantom reads) [3]
 Dwa kolejne wykonania zapytania daj¹ ró¿ne zbiory wierszy
*/

CREATE TABLE BankAccount ( -- some table
    AccID int PRIMARY KEY,
    Balance DECIMAL(10, 2)
);

--DROP TABLE BankAccount;

--[1]

INSERT INTO BankAccount 
(AccID, Balance) 
VALUES 
(1, 4200.00); -- some data for the table

-- we begin transaction, here we kinda simulate sesion 1
BEGIN TRANSACTION;

-- here we update the ballance but purposly do not commit the changes
UPDATE BankAccount
SET Balance = Balance - 420.00
WHERE AccID = 1;

-- here we check the balance after the update
SELECT Balance FROM BankAccount WHERE AccID = 1;

-- here we will kidna simulate the second session where the dirty read will show
-- we us this line so the dirty read could happend
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;

-- here we read in a dirty fashion the data
SELECT Balance FROM BankAccount WHERE AccID = 1; -- and after this session 2 ends

-- now we will rollback the change (cancel it)
ROLLBACK;

-- and after the "first session" ended we can see the balance now

SELECT Balance FROM BankAccount WHERE AccID = 1;

--DROP TABLE BankAccount;







-- [2]
INSERT INTO BankAccount -- some data for the table 
(AccID, Balance) 
VALUES 
(1, 4200.00); 

-- transaction can only read data committed by another transactions
-- data modified in one transaction is not visalbe for another untill it ends
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;

-- sesion 1 start!
BEGIN TRANSACTION;

-- acc balance
SELECT Balance FROM BankAccount WHERE AccID = 1;

-- sesion 2 start!
BEGIN TRANSACTION;

-- we modify the data
UPDATE BankAccount
SET Balance = Balance + 80.00
WHERE AccID = 1;

-- here we accualy commit the changes
COMMIT;
-- and here session 2 ends!

-- sesion 1 reads the ballance again
SELECT Balance FROM BankAccount WHERE AccID = 1;

-- and here session 1 ends!
COMMIT;

-- at the end we have this ballance
SELECT Balance FROM BankAccount WHERE AccID = 1;

-- BANG sesion 1 changes nothing but it has 2 diffrent outcomes while it is "going"
-- ;c

--DROP TABLE BankAccount;






-- [3]

-- DROP TABLE Tavern;

CREATE TABLE Tavern ( -- table for tavern customers
    OrderID int PRIMARY KEY,
    TravelerID int,
    Bill DECIMAL(10, 2)
);

INSERT INTO Tavern -- some data for our tavern table
(OrderID, TravelerID, Bill) 
VALUES 
(1, 42, 420.00),
(2, 43, 100.00),
(3, 44, 50.00);


--sesion 1 start!
-- Once a transaction reads a row, no other transaction can modify that row until the first transaction completes.
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
BEGIN TRANSACTION;

-- lets check who has at least 100 golden coins to pay in the tavern
SELECT COUNT(*) AS BillsCount from Tavern WHERE Bill > 100;
-- as a result only one traveler with ID = 42

-- session 2 start!
BEGIN TRANSACTION;

-- BANG! here we'll insert some new Traveler that has to pay MORE than 100 coins
INSERT INTO Tavern 
(OrderID, TravelerID, Bill) 
VALUES 
(4, 45, 101.00);

-- sesion 2 end!
COMMIT;

-- so we are back in sesion 1 where we read the tavern data again
SELECT COUNT(*) AS BillsCount FROM Tavern WHERE Bill > 100;
-- as a result we'll get 2 travelers 42 and 44 so there are 2 Bills over 100

-- sesion 1 end!
COMMIT;