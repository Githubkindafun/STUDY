-- so we want to have a procedure that:
-- with given id of readers returns:
-- sum of days that readers have holded the books
-- if i do understand taks 4. correctly

CREATE TYPE czytelnik_id as TABLE (
	id int 
);
GO

ALTER PROCEDURE Days_by_id
	@ids czytelnik_id READONLY
AS
BEGIN
	
	SELECT Czytelnik_ID, SUM ( Liczba_Dni ) as num_of_days
	from Wypozyczenie
	WHERE Czytelnik_ID IN (SELECT id from @ids)
	GROUP BY Czytelnik_ID

END;
GO

DECLARE @IDs czytelnik_id;

INSERT INTO @IDs (id) VALUES (1), (2), (3);

EXEC Days_by_id @ids = @IDs;


