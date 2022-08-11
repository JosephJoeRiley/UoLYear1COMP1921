#include "pgmReduce.h"

PgmImage pgmReduce(const char *read_filename, unsigned int factor, int *return_value)
{
	
	PgmImage input = pgmRead(read_filename, return_value);
	
	if(*return_value != 0)
		return createDefaultPgmObject();
	
	PgmImage output = copyPgmMetadata(input);
	
	output.width /= factor;
	output.height /= factor;
	
	int ratio_x, ratio_y;
	
	ratio_x = ratio_y = 0;
	
	//These are here 
	if(input.width > input.height)
	{
		ratio_x  = ceil((double) input.width / input.height);
	}
	else if (input.height > input.width)
	{
		ratio_y = ceil((double) input.height / input.width);
	}

	//Return if this malloc fails
	if(reMallocData(&output) != 0)
	{
		*return_value = FAILED_MALLOC;
		return createDefaultPgmObject();
	}

 	//Set each char of our output images's data         
	for(int output_row = 0; output_row < (output.height); output_row++)
		for(int output_col = 0; output_col < output.width; output_col++) 
		//This is here so that we write every <factor>th pixel of the input image to the resulting image
			output.imageData[output_col][output_row] = input.imageData[output_col * factor][output_row * factor];
	
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
	if(return_val == 0)
		pgmWrite(argv[3], result, &dummy_val);
	
	
	return printOutMsg(return_val, argv[0], "", ""); 
} 
