-- task 3 : freaky stuff


DROP INDEX index_nonclustered1 ON Ksiazka;
DROP INDEX index_nonclustered2 ON Egzemplarz;

CREATE NONCLUSTERED INDEX index_nonclustered1 ON Ksiazka(Autor); -- where
-- by indexing Autor, our database can directly locate rows matching Andrew ... 
-- instead of checking whole table ;>
CREATE NONCLUSTERED INDEX index_nonclustered2 ON Egzemplarz(Ksiazka_ID); -- join
-- by indexing Ksiazka_ID column in Egzemplarz table querry can efficiently match rows between Ksia... and Egz...


SELECT Autor
FROM Ksiazka
JOIN Egzemplarz ON Egzemplarz.Ksiazka_ID = Ksiazka.Ksiazka_ID
WHERE Autor = 'Andrew Unsworth'




-- for covering

DROP INDEX index_covering ON Egzemplarz;

CREATE NONCLUSTERED INDEX index_covering ON Egzemplarz(Ksiazka_ID, Sygnatura)
INCLUDE (Egzemplarz_ID); -- include gives us a possibility to add to index some more data wihout changing the index size
/*
A covering index is designed to include all the columns from a table (Egzemplarz) 
that are needed to satisfy a query, so that the database can retrieve the required data 
entirely from the index without accessing the base table
*/


SELECT Autor, Sygnatura
FROM Ksiazka
JOIN Egzemplarz ON Egzemplarz.Ksiazka_ID = Ksiazka.Ksiazka_ID
WHERE Autor = 'Andrew Unsworth'
