S1:
drop table if exists liczby1;
drop table if exists liczby2;
create table liczby1 ( liczba int )
create table liczby2 ( liczba int )
go


/*

S1:
begin tran
insert liczby1 values ( 1 )

S2:
begin tran
insert liczby2 values ( 1 )

S1:
update liczby2 set liczba=10

S2:
update liczby1 set liczba=10

-- i tutaj pojawia si� zakleszczenie


*/


/*
S1 locks liczby1 and attmeps to lock liczby2
S2 locks liczby2 and attemps to lock liczby1
circular reference that is the problem


*/