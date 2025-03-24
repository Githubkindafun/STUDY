-- Create a procedure for introducing a new reader
-- PESEL format validity, [1]
-- last name starts with a capital letter and consists of at least 2 letters, [2]
-- birth date is in the correct format. [3]
ALTER PROCEDURE Czytelnik_add
    @PESEL varchar(11),  -- [1]
    @LastName varchar(30), -- [2]
	@City varchar(20),
	@BirthDate varchar(10), -- [3]
	@LatestBookRental DATE
AS
BEGIN
	IF @PESEL IS NULL -- [1]
	BEGIN
		THROW 54242, 'PESEL SHOULD NOT BE NULL', 1;
	END
	ELSE IF @PESEL LIKE '%[^0-9]%'
	-- some comment to this line:
	-- '%[^0-9]%' : 
	-- %...% pattern can be anywhere in a string
	-- [^0-9] something that is not a 0 1 2 3 ... 9
	-- so besicly if in PESEL there is something that is not a number we THROW an ERROR 
	BEGIN
	    THROW 54242, 'PESEL MUST CONTAIN ONLY NUMBERS', 1;
	END;

	IF @LastName IS NULL -- [2]
	BEGIN

		THROW 54242, 'LAST NAME SHOULD NOT BE NULL', 1;

	END
	ELSE IF DATALENGTH(@LastName) < 2
	BEGIN

		THROW 54242, 'LAST NAME SHOULD BE AT LEAST 2 LETTERS LONG', 1;

	END
	ELSE IF LEFT( @LastName, 1 ) COLLATE Latin1_General_Bin != UPPER( LEFT( @LastName, 1  ) ) COLLATE Latin1_General_Bin -- [2]
	BEGIN

		THROW 54242, 'LAST NAME SHOULD START WITH AN CAPITAL LETTER DUMMY', 1;

	END;

	
	IF TRY_CONVERT( DATE, @BirthDate ) IS NULL -- [3] -- DATE is by defult in format YYYY-MM-DD
	BEGIN

		THROW 54242, 'THERE IS SOMETHING WRONG WITH THE BIRTHDATE :C', 1;

	END

	INSERT
		INTO Czytelnik (
			PESEL,
			Nazwisko,
			Miasto,
			Data_Urodzenia,
			Ostatnie_Wypozyczenie )
		VALUES (
			@PESEL,
			@LastName,
			@City,
			@BirthDate,
			@LatestBookRental )
		
END;

-- some test data :

EXEC Czytelnik_add -- thats some fine data
    @PESEL = '12345678910', 
    @LastName = 'Mickiewicz', 
    @City = 'Warszawa', 
    @BirthDate = '1855-11-26', 
    @LatestBookRental = NULL;

EXEC Czytelnik_add -- too long PESEL
    @PESEL = '123456789A', -- here i discovered that varchar(11) just cuts first 11 chars 
    @LastName = 'Mickiewicz', 
    @City = 'Warszawa', 
    @BirthDate = '1855-11-26', 
    @LatestBookRental = NULL;

EXEC Czytelnik_add -- last name starts with non CAPITAL letter
    @PESEL = '12345678910', 
    @LastName = 'mickiewicz', 
    @City = 'Warszawa', 
    @BirthDate = '1855-11-26', 
    @LatestBookRental = NULL;

EXEC Czytelnik_add -- last name is shorter than 2 letters
    @PESEL = '12345678910', 
    @LastName = 'M', 
    @City = 'Warszawa', 
    @BirthDate = '1855-11-26', 
    @LatestBookRental = NULL;

EXEC Czytelnik_add -- DATE WRONG !
    @PESEL = '12345678910', 
    @LastName = 'Mickiewicz', 
    @City = 'Warszawa', 
    @BirthDate = '1855-26-11', 
    @LatestBookRental = NULL;

EXEC Czytelnik_add -- DATE WRONG !
    @PESEL = '12345678910', 
    @LastName = 'Mickiewicz', 
    @City = 'Warszawa', 
    @BirthDate = '1855-02-29', 
    @LatestBookRental = NULL;

DELETE FROM Czytelnik -- to clean
WHERE Nazwisko = 'Mickiewicz';