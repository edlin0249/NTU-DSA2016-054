關於cancel只要花O(log(n))的部份,我是用stl:set把要cancel的bidid先存起來（set的insert()是O(log(n))）然後每次交易進行前先看看買方和賣方的bidid有無出先在要cancel的stl:set裡（set的find()是O(log(n))）,有則remove,沒有則進行交易,這要即可讓cancel這個動作為O(log(n))
