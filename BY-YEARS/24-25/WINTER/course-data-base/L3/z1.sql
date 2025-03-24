-- Exercise 1. : Making an backup data table, without/with cursors [1]/[2]


-- Adventure db

--[1]
SELECT * INTO SalesLT.Customer_Backup FROM SalesLT.Customer 
-- SLECET INTO creates a new table and insert into it the * data from the SalesLT.Customer


--DROP TABLE SalesLT.Customer_Backup

--[2]



SELECT * INTO SalesLT.Customer_Backup_Cursor FROM SalesLT.Customer WHERE 1 = 0;
-- WHERE 1 = 0 means that we take only the schema for the table. No data is copied.

SET IDENTITY_INSERT SalesLT.Customer_Backup_Cursor ON;

DECLARE 
	@CustomerID int, 
	@NameStyle bit,
	@Title nvarchar(8),
	@FirstName nvarchar(50),
    @MiddleName nvarchar(50),
    @LastName nvarchar(50),
    @Suffix nvarchar(10),
    @CompanyName nvarchar(128),
    @SalesPerson nvarchar(256),
    @EmailAddress nvarchar(50),
    @Phone nvarchar(25),
    @PasswordHash varchar(128),
    @PasswordSalt varchar(10),
    @rowguid uniqueidentifier,
    @ModifiedDate datetime,
    @CreditCardNumber int;

DECLARE freaky_cursor 
CURSOR FOR SELECT 
	CustomerID,
	NameStyle,
    Title,
    FirstName,
    MiddleName,
    LastName,
    Suffix,
    CompanyName,
    SalesPerson,
    EmailAddress,
    Phone,
    PasswordHash,
    PasswordSalt,
    rowguid,
    ModifiedDate,
    CreditCardNumber
FROM SalesLT.Customer

OPEN freaky_cursor;

FETCH NEXT FROM freaky_cursor 
INTO
	@CustomerID, 
	@NameStyle,
	@Title,
	@FirstName,
    @MiddleName,
    @LastName,
    @Suffix,
    @CompanyName,
    @SalesPerson,
    @EmailAddress,
    @Phone,
    @PasswordHash,
    @PasswordSalt,
    @rowguid,
    @ModifiedDate,
    @CreditCardNumber
;

WHILE @@FETCH_STATUS = 0
BEGIN
	INSERT INTO SalesLT.Customer_Backup_Cursor 
	(
	CustomerID,
	NameStyle,
    Title,
    FirstName,
    MiddleName,
    LastName,
    Suffix,
    CompanyName,
    SalesPerson,
    EmailAddress,
    Phone,
    PasswordHash,
    PasswordSalt,
    rowguid,
    ModifiedDate,
    CreditCardNumber
	) VALUES (
	@CustomerID, 
	@NameStyle,
	@Title,
	@FirstName,
    @MiddleName,
    @LastName,
    @Suffix,
    @CompanyName,
    @SalesPerson,
    @EmailAddress,
    @Phone,
    @PasswordHash,
    @PasswordSalt,
    @rowguid,
    @ModifiedDate,
    @CreditCardNumber
	);

	FETCH NEXT FROM freaky_cursor INTO 
		@CustomerID, 
		@NameStyle,
		@Title,
		@FirstName,
		@MiddleName,
		@LastName,
		@Suffix,
		@CompanyName,
		@SalesPerson,
		@EmailAddress,
		@Phone,
		@PasswordHash,
		@PasswordSalt,
		@rowguid,
		@ModifiedDate,
		@CreditCardNumber;
END

CLOSE freaky_cursor;
DEALLOCATE freaky_cursor;

SET IDENTITY_INSERT SalesLT.Customer_Backup_Cursor OFF;

--DROP TABLE SalesLT.Customer_Backup_Cursor;