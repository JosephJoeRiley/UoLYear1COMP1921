#include "pgmb2a.h"

/*	Outline of pgmb2a.c
		Read a pgmObject from the input file 
		Change the magic number of the resultant object from 5 to 2
		Write the same object to the output file 
    */
int main(int argc, char** argv)
{
	if(argc == 1)
            return printOutMsg(USAGE_ERROR, argv[0], "", "");
    else if(argc != 3)
            return printOutMsg(BAD_ARG_NO, argv[0], "", "");

	int errorReturn = 0;
    PgmImage a = pgmRead(argv[1], &errorReturn);
    if(errorReturn != 0)
        return printOutMsg(errorReturn, argv[0], argv[1], "");
    
    if((convertB2A(argv[2], a, &errorReturn)).magicNumber[1] != '2')
        return printOutMsg(errorReturn, argv[0], argv[2], "");
    else 
        return printOutMsg(CONVERTED, argv[0], "", "");
}
