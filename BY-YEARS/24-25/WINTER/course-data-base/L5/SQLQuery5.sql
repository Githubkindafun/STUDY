-- task 3 : freaky stuff
SELECT Autor
FROM Ksiazka
JOIN Egzemplarz ON Egzemplarz.Ksiazka_ID = Ksiazka.Ksiazka_ID
WHERE Autor = 'Helen Feddema'

DROP INDEX index_nonclustered1 ON Ksiazka;
DROP INDEX index_nonclustered2 ON Egzemplarz;

CREATE NONCLUSTERED INDEX index_nonclustered1 ON Ksiazka(Autor); -- where

CREATE NONCLUSTERED INDEX index_nonclustered2 ON Egzemplarz(Ksiazka_ID); -- join


-- for non cluster

SELECT Autor, Sygnatura
FROM Ksiazka
JOIN Egzemplarz ON Egzemplarz.Ksiazka_ID = Ksiazka.Ksiazka_ID
WHERE Autor = 'Helen Feddema'

DROP INDEX index_covering ON Egzemplarz;

CREATE NONCLUSTERED INDEX index_covering ON Egzemplarz(Ksiazka_ID, Sygnatura)
INCLUDE (Egzemplarz_ID); -- what we join