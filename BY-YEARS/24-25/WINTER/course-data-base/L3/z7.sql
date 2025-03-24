-- exercise 7. : triggers books etc.



ALTER TRIGGER freaky_trigger
ON Egzemplarz
INSTEAD OF INSERT -- aaaa see here we can use what we've learnd in the exercise 6. ;>
AS
BEGIN
    
	DECLARE @KsiazkaID int;

	SELECT @KsiazkaID = Ksiazka_ID
	from inserted

	IF (SELECT COUNT(*) from Egzemplarz WHERE Ksiazka_ID = @KsiazkaID) >= 5
	BEGIN
		THROW 54242, 'Oh my dear friend the max number of speciman is 5, no more please', 1;
	END
	ELSE
	BEGIN 
		INSERT INTO Egzemplarz (
		--Egzemplarz_ID,
		Sygnatura,
		Ksiazka_ID)
		SELECT 
		--Egzemplarz_ID,
		Sygnatura, Ksiazka_ID from inserted;
	END
END;

-- some test data
SELECT * from Egzemplarz

INSERT INTO Egzemplarz (
	--Egzemplarz_ID,
	Sygnatura,
	Ksiazka_ID) 
	VALUES (
	--16,
	'S0017',
	3);

-- Lets go it works Yippie