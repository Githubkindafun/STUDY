/* here we created new table
CREATE TABLE TEST 
(	
	ID int IDENTITY(1000, 10),
	whatever int
);
*/
insert into TEST values (42), (43), (44), (45) 

Select @@IDENTITY as '@@', IDENT_CURRENT('TEST') as 'curr'
-- @@IDENTITY - newest added globaly ID
-- IDENT_CURRENT 'table_name' return last ID from current table 