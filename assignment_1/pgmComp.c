#include "pgmComp.h"

//Returns 1 if the two PgmImages are equal,
//0 otherwise
int comparePgms(const char *aDir, const char* bDir, int *errorReturn)
{	
	//Read in the first image and return 10
	//if error_return is assigned a faliure value.
	PgmImage a = pgmRead(aDir, errorReturn);
	if(*errorReturn)
	{
		return printOutMsg(*errorReturn, "./pgmComp", aDir, "");
		return 10;
	}
	//Same but with the second
	PgmImage b = pgmRead(bDir, errorReturn);
	if(*errorReturn)
	{	
		return printOutMsg(*errorReturn, "./pgmComp", bDir, "");
		return 10;
	}
	
	//If there's a mis-match, then we make sure 
	//we're comparing two binary images since it's faster
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
	//This is explained above the declaration
	return (a.imageData == b.imageData);
}

int main(int argc, char** argv)
{
	if(argc == 1)
            return printOutMsg(USAGE_ERROR, argv[0], "", "");
    else if(argc != 3)
            return printOutMsg(BAD_ARG_NO, argv[0], "", "");

	int errorReturn = 0;

	//We already printed any error messages in our compare function
	//so no need to here
	if (comparePgms(argv[1], argv[2], &errorReturn) > 1)
		return errorReturn;	
	else
		return printOutMsg(errorReturn - 1, argv[0], "", "");
}
