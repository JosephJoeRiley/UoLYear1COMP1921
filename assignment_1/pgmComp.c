#include "pgmComp.h"

int comparePgms(const char *aDir, const char* bDir, int *errorReturn)
{
	PgmImage a = pgmRead(aDir, errorReturn);
	if(*errorReturn)
	{
		printOutMsg(*errorReturn, "./pgmComp", aDir, "");
		return 10;
	}
	PgmImage b = pgmRead(bDir, errorReturn);
	if(*errorReturn)
	{
		printOutMsg(*errorReturn, "./pgmComp", bDir, "");
		return 10;
	}
	
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

	if (comparePgms(argv[1], argv[2], &errorReturn) > 1)
		return printOutMsg(errorReturn, argv[0], "", "");	
	else
		return printOutMsg(errorReturn - 1, argv[0], "", "");
}
