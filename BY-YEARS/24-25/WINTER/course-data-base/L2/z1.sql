--zad1
--CREATE FUNCTION Days_number (@amount_of_days int)
ALTER FUNCTION Days_number (@amount_of_days int)

RETURNS TABLE
AS
RETURN (

SELECT 
	PESEL,
	COUNT (Egzemplarz_ID) as amount

FROM 
	Czytelnik
JOIN 
	Wypozyczenie on Czytelnik.Czytelnik_ID = Wypozyczenie.Czytelnik_ID

WHERE
	Liczba_Dni >= @amount_of_days 
GROUP BY PESEL
);



SELECT *
FROM Days_number (10);

