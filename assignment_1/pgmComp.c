#include "pgmComp.h"

int comparePgms(const char *aDir, const char* bDir, int *errorReturn)
{
	PgmImage a = pgmRead(aDir, errorReturn);
	PgmImage b = pgmRead(bDir, errorReturn);
	
	const char *tempDir = "tempComp.pgm";
	if(a.magicNumber[1] != b.magicNumber[1])
	{
		if(a.magicNumber[1] == '2') 
		{
			a  = convertA2B(tempDir, a, errorReturn);
			remove(tempDir);
		}
		else if(b.magicNumber[1] == '2') 
		{
			b = convertA2B(tempDir, b, errorReturn);
			remove(tempDir);
		}
	}
		return (a.imageData == b.imageData);
}

int main(int argc, char** argv)
{
	if(argc == 1)
            return printOutMsg(USAGE_ERROR, argv[0], "", "");
    	else if(argc != 3)
            return printOutMsg(BAD_ARG_NO, argv[0], "", "");

	int errorReturn = 0;


	comparePgms(argv[1], argv[2], &errorReturn);	
	
	
	if(errorReturn > 0)
		return printOutMsg(errorReturn, argv[0], "", "");
	else
		return printOutMsg(errorReturn - 1, argv[0], "", "");
}
