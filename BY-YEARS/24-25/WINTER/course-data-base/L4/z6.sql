-- z6 : locking hits concept : example

-- hints : Nadpisuje zachowanie zdefiniowane przez bie¿¹cy poziom izolacji : Overrides behaviour defined by current isolation level

DROP TABLE z5v3;

CREATE TABLE zad5 
(number int)

SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;

BEGIN TRANSACTION

INSERT into 
zad5
VALUES
(42)

WAITFOR DELAY '00:00:05'

COMMIT