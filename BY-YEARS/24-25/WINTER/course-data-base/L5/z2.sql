-- task 2 : 

-- [I]
--SET STATISTICS TIME ON; -- [time]
DECLARE @StartTime DATETIME2 = SYSDATETIME();

SELECT DISTINCT c.PESEL, c.Nazwisko 
FROM Egzemplarz e 
JOIN Ksiazka k ON e.Ksiazka_ID=k.Ksiazka_ID 
JOIN Wypozyczenie w ON e.Egzemplarz_ID=w.Egzemplarz_ID 
JOIN Czytelnik c ON c.Czytelnik_ID = w.Czytelnik_ID;

DECLARE @EndTime DATETIME2 = SYSDATETIME();
SELECT DATEDIFF(MILLISECOND, @StartTime, @EndTime) AS ExecutionTimeInMilliseconds;
--SET STATISTICS TIME OFF; -- [time]
/*
multiple joins
after retriving all of the rows, Distinct removes all of the duplicates
that could be expensive 
*/

-- [II]
DECLARE @StartTime2 DATETIME2 = SYSDATETIME();

--SET STATISTICS TIME ON; -- [time]
SELECT c.PESEL, c.Nazwisko 
FROM Czytelnik c WHERE c.Czytelnik_ID IN 
(SELECT w.Czytelnik_ID FROM Wypozyczenie w 
JOIN Egzemplarz e ON e.Egzemplarz_ID=w.Egzemplarz_ID 
JOIN Ksiazka k ON e.Ksiazka_ID=k.Ksiazka_ID)

DECLARE @EndTime2 DATETIME2 = SYSDATETIME();
SELECT DATEDIFF(MILLISECOND, @StartTime2, @EndTime2) AS ExecutionTimeInMilliseconds;
--SET STATISTICS TIME OFF; -- [time]
/*
subquerry retrives a list of czytelnik_ID
outer querry takes rows from czytelnik where czytelnik_ID matches IDs returned
	by the subquerry
*/

--[III]
DECLARE @StartTime3 DATETIME2 = SYSDATETIME();

SELECT c.PESEL, c.Nazwisko
FROM Czytelnik c WHERE c.Czytelnik_ID IN 
(SELECT w.Czytelnik_ID FROM Wypozyczenie w WHERE w.Egzemplarz_ID IN 
(SELECT e.Egzemplarz_ID FROM Egzemplarz e
JOIN Ksiazka k ON e.Ksiazka_ID = k.Ksiazka_ID))

DECLARE @EndTime3 DATETIME2 = SYSDATETIME();
SELECT DATEDIFF(MILLISECOND, @StartTime3, @EndTime3) AS ExecutionTimeInMilliseconds;