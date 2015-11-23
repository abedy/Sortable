Compile from command line using:
gcc main.c Str.c Products.c Memory.c Listings.c -o Challenge

You may need to make the program executable using:
chmod +x Challenge

After compilation run the program with:
./Challenge pathToProductsFile pathToListingsFile [pathToOutputFile]

ex: ./challenge products.txt listings.txt output.txt

if no output file is specified, output will be sent to stdout. (which can be redirected to a file)

