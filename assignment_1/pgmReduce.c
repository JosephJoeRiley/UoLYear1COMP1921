#include "pgmReduce.h"

PgmImage pgmReduce(const char *read_filename, unsigned int factor, int *return_value)
{
	PgmImage input = pgmRead(read_filename, return_value);
	PgmImage output = copyPgm(input);
	output.width /= factor;
	output.height /= factor;
	unsigned int output_dataLen = output.width * output.height;
	output.imageData = (unsigned char *) malloc(output_dataLen * sizeof(unsigned char *));
	//Return if this malloc fails
	if(output.imageData == NULL)
	{
		*return_value = FAILED_MALLOC;
		return createDefaultPgmObject();
	}

 	//Set each char of our output images's data         
	for(int index = 0; index < (output.width * output.height); index++)
	{
		//This formula we're using with index 
		//jumps to every pixel that
		//is divisble by the given factor
		output.imageData[index] = input.imageData[
		index * (factor - 1)];

	}
	
	//return our pgm 
	return output;
} 

int main(int argc, char** argv)
{
	int dummy_val, return_val = dummy_val = -1;
    
	if(argc == 0) 
		return printOutMsg(USAGE_ERROR, argv[0], "", "");
	else if (argc != 4)
		return printOutMsg(BAD_ARG_NO, argv[0], "", "");
	
	unsigned int my_factor, counter, numlen;
	my_factor = counter = numlen = 0;
	while(argv[2][++numlen]);
	while(counter <= numlen)
	{
		char c = (argv[2][counter++]);
		unsigned int digit = c - '0';
		
		for(int index = 1; index < numlen; index++)
			digit *= 10;
		
		my_factor += digit;
		numlen--;
	}	

	PgmImage result = pgmReduce(argv[1], my_factor, &return_val);
	pgmWrite(argv[3], result, &dummy_val);

	printOutMsg(return_val, argv[0], argv[1], argv[3]);
	return printOutMsg(return_val, argv[0], "", ""); 
} 
