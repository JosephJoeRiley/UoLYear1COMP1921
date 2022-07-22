#include "pgmComp.h"

int comparePgms(PgmImage a, PgmImage b)
{
	int identical;
	
}

int main(int argc, char** argv)
{
	if(argc != 3)
		return printOutMsg(USAGE_ERROR, argv[0], "", "")#include "pgmComp.h"
	
	int errorReturn = 0;

	PgmImage a = pgmRead(argv[1], &errorReturn);
	if(errorReturn != 0)
		return printOutMsg(errorReturn, argv[0], argv[1], "");
	
	PgmImage b = pgmRead(argv[2], &errorReturn);
	if(errorReturn != 0)
		return printOutMsg(errorReturn, argv[0], argv[2], "");
	
	errorReturn = -1;
	
	errorReturn = -2;
	
	return printOutMsg((comparePgms(a,b) - 1), argv[0], "", "");
}
