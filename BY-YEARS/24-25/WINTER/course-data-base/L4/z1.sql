-- z1
--Explain shortly meaning and reasoning for ACID in transactions.
/*
ACID :
	1. Atomicity
	2. Consistency
	3. Isolation
	4. Durability
*/
/*
1.
meaning : Transaction is treated as a single unit i.e. either all succed or all fail
reasoning : Prevents half-done updates it is always all or nothing

2.
meaning : Transaction !must! take database from one valid state to another
while following all database rules (f.e. triggers, constrains)
reasoning : Ensures data integrity through the proccess of transaction

3.
meaning : Transactions are islolated from each other meaning changes done by one are not 
"visable" for another unless one is done.
reasoning : Prevents possible interference between transactions that may led to problems

4.
meaning : After Transaction is commited the changes are pernament.
reasoning : Ensures that after Transaction completion the changes won't be lost unexpectadly

kinda like Atomic stuff in java
*/









