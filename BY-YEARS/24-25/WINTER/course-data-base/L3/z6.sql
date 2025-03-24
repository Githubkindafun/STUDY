-- exercise 6 : some tutorial about INSTEAD OF
-- i guess i have to get the database necessary to do it but the rest is quite easy
-- INSTEAD OF litearlly just gives as a possibilty to do something instead of f.e. INSERT
-- basicly thats all

CREATE TABLE production.brand_approvals( -- new table for storing pending approvals
    brand_id INT IDENTITY PRIMARY KEY,
    brand_name VARCHAR(255) NOT NULL
);


CREATE VIEW production.vw_brands -- view 
AS
SELECT
    brand_name,
    'Approved' approval_status
FROM
    production.brands
UNION
SELECT
    brand_name,
    'Pending Approval' approval_status
FROM
    production.brand_approvals;

CREATE TRIGGER production.trg_vw_brands -- once a row is inserted into the view we need to direct it to the approvals via INSEAD OF trigger
ON production.vw_brands
INSTEAD OF INSERT
AS
BEGIN
    SET NOCOUNT ON;
    INSERT INTO production.brand_approvals ( 
        brand_name
    )
    SELECT
        i.brand_name
    FROM
        inserted i
    WHERE
        i.brand_name NOT IN (
            SELECT 
                brand_name
            FROM
                production.brands
        );
END


INSERT INTO production.vw_brands(brand_name) -- here we litteraly insert a new brand
VALUES('Eddy Merckx');

SELECT -- we can now see a new row
	brand_name,
	approval_status
FROM
	production.vw_brands;

SELECT 
	*
FROM 
	production.brand_approvals;