
-- [1]
CREATE TABLE firstnames (
    id int PRIMARY KEY NOT NULL IDENTITY(1,1),	-- incrementing id by a 1 start with 1
    FirstName VARCHAR(42) NOT NULL
);

CREATE TABLE lastname (
    id int PRIMARY KEY NOT NULL IDENTITY(1,1),	-- incrementing id by a 1 start with 1
    LastName VARCHAR(420) NOT NULL
);

CREATE TABLE fldata (  
    FirstName VARCHAR(42) NOT NULL, 
	LastName VARCHAR(420) NOT NULL,
	PRIMARY KEY (FirstName, LastName)
);

-- [2]
INSERT INTO firstnames (FirstName) values 
	('Jon'),
	('Tyrion'),
	('Sam'),
	('Aegon')
;

INSERT INTO lastname (LastName) values 
	('Snow'),
	('Lannister'),
	('Tarly'),
	('Targaryen')
;
 -- [3]

ALTER PROCEDURE rand_pairs
    @num_of_pairs int
AS -- beginning of the proceuder
BEGIN
	-- if n > len(fs) * len(ls) 
	IF  ( (SELECT COUNT(*) FROM firstnames) * (SELECT COUNT(*) FROM lastname) ) < @num_of_pairs
	BEGIN
		THROW 54242, 'Too few data to create expected pairs', 1;
	END;

    TRUNCATE TABLE fldata; -- cleans current data and resets the id

	DECLARE @counter int = 0;
	WHILE @counter < @num_of_pairs
	BEGIN
		DECLARE  @FirstName Varchar(20);
		DECLARE  @LastName Varchar(30);
		
		SET @FirstName = (SELECT TOP 1 FirstName FROM firstnames ORDER BY NEWID());
		SET @LastName = (SELECT TOP 1 LastName FROM lastname ORDER BY NEWID());
		--here we basicly give variables the value and "ORDER BY NEWID()" esures us that the 
		--choosen result will be random

		IF NOT EXISTS ( SELECT 1 FROM fldata WHERE FirstName = @FirstName AND LastName = @LastName )
		BEGIN
			INSERT 
			INTO fldata (FirstName, LastName)
			VALUES (@FirstName, @LastName);
			SET @counter = @counter + 1;
		END;
	END;
END;

EXECUTE rand_pairs @num_of_pairs = 16;

SELECT * 
from fldata;

