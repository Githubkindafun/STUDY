Select 
parent.Name AS ParentCategory,
child.Name AS SubCategory

From SalesLT.ProductCategory child

Join SalesLT.ProductCategory parent ON child.ParentProductCategoryID = parent.ProductCategoryID;

-- child to nazwa tabeli tak samo 

