Select ProductModel.Name, count(Product.ProductID) as amount   /* count(*) domysla sie po czym*/                                                                              
from SalesLT.ProductModel
join SalesLT.Product on SalesLT.ProductModel.ProductModelID = SalesLT.Product.ProductModelID
group by ProductModel.Name
having count(Product.ProductID) > 1
--zle