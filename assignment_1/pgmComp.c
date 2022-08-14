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
		//These probably wont erase the PgmObjects since 
		//the tempdir and the directory of the original image are different
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

	//If the image data comes out to be the same we 
	//return identical, but first of all we check if
	//the dimensions are the same so we're definitly
	//involving all of image b in the comparison
	if(a.width == b.width && a.height == b.height && 
	memcmp(a.imageData, b.imageData, a.width * a.height))
		return IDENTICAL;
	//If image b does not have the same size, 
	//naturally the two images differ
	else
		return DIFFERENT;
}

int main(int argc, char** argv)
{	
	if(argc == 1)
            return printOutMsg(USAGE_ERROR, argv[0], "", "");
    else if(argc != 3)
            return printOutMsg(BAD_ARG_NO, argv[0], "", "");

	int errorReturn = 0;

	//If comparePgms returns neither a 1 nor a 0, we return the error value
	int result = 0;
	if ((result = comparePgms(argv[1], argv[2], &errorReturn)) > 1)
		return errorReturn;	
	//Otherwise if it has returned 1 or 0, we return the result of the comparison
	else
		return printOutMsg(result - 1, argv[0], "", "");
}
