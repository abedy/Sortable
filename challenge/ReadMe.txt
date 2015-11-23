Compile from command line using:
gcc main.c Str.c Products.c Memory.c Listings.c -o Challenge

You may need to make the program executable using:
chmod +x Challenge

After compilation run the program with:
./Challenge pathToProductsFile pathToListingsFile [pathToOutputFile]

ex: ./challenge products.txt listings.txt output.txt

if no output file is specified, output will be sent to stdout. (which can be redirected to a file)


Please note:
I purposely tried to avoid making non-exact matches (not including colour).
For example, listings for accessories for a product (lens, bag, etc) should not match with that product unless the listing with accessories is a package that includes the product itself.

I hope I got the JSON output right, an example output line is:
{"product_name":"Toshiba_PDR-M60","listings":[{"title":"Toshiba PDR-M60 2.3MP Digital Camera w/ 2x Optical Zoom","manufacturer":"Toshiba","currency":"USD","price":"19.50"}]}