-- rodzaje blokad

--USE Test;

drop table if exists liczby;
go
create table liczby ( liczba int );
go
insert liczby values ( 10 );
go

-- 1 --
set transaction isolation level repeatable read; -- 2rd strongest prevents dirty read and non-repeatable
begin transaction

-- w drugim po��czeniu robimy update: 
--update liczby set liczba=4
-- ogl�damy blokady: sp_lock

select * from liczby

-- ponownie w drugim po��czeniu robimy update: 
--update liczby set liczba=4
-- ogl�damy blokady: sp_lock

commit

/*
first "connection":
spid	dbid	ObjId	IndId	Type	Resource	Mode	Status
62	1	1787153412	0	TAB	                          IS	GRANT
62	32767	-571204656	0	TAB	                       Sch-S	GRANT
MODE is what interests us here becouse it is the lock type
Intent locks 
 Blokady zak�adane na poziomie PAGE lub TABLE 
 Lock Manager mo�e dzi�ki nim lepiej zarz�dza� blokadami 
 Wykorzystywane w eskalacji blokad 
 Mamy r�ne warianty 
 IS � transakcja zamierza czyta� cz�� zasob�w przez na�o�enie na nie blokady S
 IS : An intent shared lock, indicating that the transaction intends to acquire shared locks at a lower level.

Schema locks 
 Schema stability (Sch-S) 
 U�ywany podczas kompilacji zapyta� 
 Uniemo�liwia wykonanie polece�, kt�re wymagaj� Sch-M (schema modify)

THEN WE HAVE second "connection"
 spid	dbid	ObjId	IndId	Type	Resource	Mode	Status
62	1	260195977	0	RID	1:365:0                         	X	GRANT
62	1	260195977	0	PAG	1:365                           	IX	GRANT
62	1	260195977	0	TAB	                                	IX	GRANT
62	1	1787153412	0	TAB	                                	IS	GRANT
62	32767	-571204656	0	TAB									Sch-S	GRANT
	
	IX � transakcja zamierza modyfikowa� cz�� zasob�w poprzez na�o�enie na nie blokady X
Exclusive (X) 
 Daj� wy��czno�� dost�pu do zasobu 
 Niezb�dne dla operacji INSERT, UPDATE, DELETE 
 Tylko jedna transakcja mo�e na�o�y� tak� blokad�
	
*/



-- 2 --
set transaction isolation level serializable; -- 1st sstrongest prevents phantom/dirty/non

insert liczby values ( 10 );

begin transaction

-- w drugim po��czeniu robimy insert: 
insert liczby values(151)
-- ogl�damy blokady: sp_lock

select * from liczby

-- ponownie w drugim po��czeniu robimy insert: 
insert liczby values(151)
-- ogl�damy blokady: sp_lock

commit


/*
MANY, accualy quite a lot, locks
becouse we insert data and serializable isolation leevel
couse number of locks to be high due to the locking on basicly everything
*/
















-- 3 --
-- sprawdzamy opcj� Allow Snapshot Isolation
-- ALTER DATABASE AdventureWorksLT SET ALLOW_SNAPSHOT_ISOLATION ON
set transaction isolation level snapshot;
begin transaction

select * from liczby

-- ponownie w drugim po��czeniu robimy update: update liczby set liczba=6
-- ogl�damy blokady: sp_lock

update liczby set liczba=7

select * from liczby

commit
