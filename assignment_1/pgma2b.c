#include "pgma2b.h"

/*	Outline of pgma2b.c
		Read a pgmObject from the input file 
		Change the magic number of the resultant object from 2 to 5
		Write the same object to the output file 
    */
PgmImage convertA2B(const char *outDir, PgmImage a, int *errorReturn)
{
    a.magicNumber[1] = '5';
    pgmWrite(outDir, a, errorReturn);
    printOutMsg(*errorReturn, "./pgma2b", a.filename, outDir);
    return a; 
}

int main(int argc, char** argv)
{
    int return_value = -1;
    if(argc != 3)
    {
        printOutMsg(USAGE_ERROR, argv[0], "", "");
    }
    convertA2B(argv[2], pgmRead(argv[1], &return_value), &return_value);
    return printOutMsg(return_value, argv[0], "", "");
}
