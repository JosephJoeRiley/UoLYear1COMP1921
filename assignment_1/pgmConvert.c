#include "pgmConvert.h"

/*	PGMCONVERT OUTLINE
Change the input pgm's magic number,
write the image with a changed magic number 
to the directory given 
*/

PgmImage convertA2B(const char *outDir, PgmImage a, int *errorReturn)
{
    a.magicNumber[1] = '5';
    pgmWrite(outDir, a, errorReturn);
    if(*errorReturn != SUCCESS)
    	return createDefaultPgmObject();
    return pgmRead(outDir, errorReturn); 
}

PgmImage convertB2A(const char *outDir, PgmImage a, int *errorReturn)
{
    a.filename = outDir;
    a.magicNumber[1] = '2';
    pgmWrite(outDir, a, errorReturn);
    if(*errorReturn != SUCCESS)
    	return createDefaultPgmObject();
    return pgmRead(outDir, errorReturn); 
}
