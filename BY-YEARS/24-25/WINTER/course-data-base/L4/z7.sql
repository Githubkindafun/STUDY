-- z7 : Based on a small (theoretical) example, explain the difference between pessimistic and optimistic concurrency control mechanism.


-- Pessimistic
/*
Basicly whenever the data is read or written it uses locks to ensure that noone interfere
example :
-- session 1 starts!
BEGIN TRANSACTION;

-- row is locked for update
UPDATE ManaPool
SET Mana = Mana - 10
WHERE CharacterID = 1;
-- data is updated

-- if there would be any transaction now it would be blocked until we commit;
/*
.
.
.
*/
--commit
*/

-- Optimistic
/*
Basicly it is optimimistic so checks if there are conflicts only when neccesary at the end before the commit
example : 

BEGIN TRANSACTION 1
Mana = 100
Version = 1

-- we update the quantity
Mana = Mana - 10 

-- here we check if we can commit thats all (if the version is "right")

IF Version is 1 THEN
	UPDATE ManaPool SET Mana = 90, Version = 2
        COMMIT
    ELSE
        -- BANG! conflict here so we rollback 
        ROLLBACK
END




*/












