#include "pgmb2a.h"

/*	Outline of pgmb2a.c
		Read a pgmObject from the input file 
		Change the magic number of the resultant object from 5 to 2
		Write the same object to the output file 
    */
PgmImage convertB2A(const char *outDir, PgmImage a, int *errorReturn)
{
    a.magicNumber[1] = '2';
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
    convertB2A(argv[2], pgmRead(argv[1], &return_value), &return_value);
    return printOutMsg(return_value, argv[0], "", "");
}
