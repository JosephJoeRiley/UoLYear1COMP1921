#include "pgmConvert.h"

PgmImage convertA2B(const char *outDir, PgmImage a, int *errorReturn)
{
    a.magicNumber[1] = '5';
    pgmWrite(outDir, a, errorReturn);
    return a; 
}

PgmImage convertB2A(const char *outDir, PgmImage a, int *errorReturn)
{
    a.magicNumber[1] = '2';
    pgmWrite(outDir, a, errorReturn);
    return a; 
}
