set nocount on

drop table if exists liczby
go
create table liczby( nr int primary key, liczba int )
go
declare @a int
set @a=1
while ( @a<=60)
begin
  insert liczby values ( @a, @a )
  set @a=@a+1
end
go

declare @x int
set @x=10

-- Do wykonania 3 razy (ka¿de z osobna, analizujemy wyniki: results i messages)
	--declare c cursor for select liczba from liczby where liczba<=@x
--declare c cursor static for select liczba from liczby where liczba<=@x
declare c cursor keyset for select liczba from liczby where liczba<=@x 

set @x=20

open c

declare @aux int, @licznik int
set @licznik=2

print 'Kolejne liczby z kursora:'
fetch next from c into @aux 

while ( @@fetch_status=0 )
begin
  print @aux;
  print 'Liczba: ' + cast(@aux as varchar)
  print 'Licznik:' + cast(@licznik as varchar)
  delete from liczby where liczba=@licznik
  fetch next from c into @aux 
  set @licznik=@licznik+2
end

print 'Status ostatniej instrukcji fetch: ' + cast(@@fetch_status as varchar)
close c
deallocate c

select * from liczby where liczba<=10







/*
Cursors:

Static Cursor:
a temporary tempdb table is created for the resulting cursor data and operations are performed on this table.

Dynamic:
we work on a live table.

Keyset:
only key values are copied to a temporary table in tempdb.
as far as i do understand it works like dynamic but on a part of a table that was "snapshoted"
and we will see only this part. We can not change the keys tho.

(this is from the lecture about cursors i guess it says everything in a nice and a simple way)


About the task it self:
[1] works fine
[2] same
[3] would not go further becuse fetch status = -2 == The row fetched is missing.

*/