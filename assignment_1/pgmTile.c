#include "pgmTile.h"

char *int_to_char(unsigned int input)
{
	unsigned int order = 10000, char_array_len = 0;
	while(input % order != 0) 
	{
		order /= 10;
		++char_array_len;
	}
	char char_array[char_array_len];
	//i: the index of the character array
	//
	//j: the reference to the inputted integer 
	//from which we get our digit (to place in  
	//the ith place of the char array)
	//
	//k: the offset for the digit to make it 
	//a single digit that can be turned into a char
	for(int i = 0, j = input, k = order
	i < char_array_len, j > 0, k <= 1;
	i++, j %= k, k /= 10)
	{
		char_array[i] = (char) (j / k) + '0';		
	}
	return char_array;	
}	
const char *createFileName(const char *spec, unsigned int x, unsigned int y)
{
	//Since spec is already a const char, we know its size is a 
	//valid size for this character array so this malloc will never fail
	char *title = (char *) malloc(strlen(spec) * sizeof(char));
	//Create a for loop with two variables:
	//index is not the index of title but the index of spec
	//the loop condition doesn't really matter since we will always
	//stop reading from spec when we return a 'c', the first character of 'column'
	//We don't want spec's index to be inside the loop since we'll want to 
	//access it later 
	int index = 0;
	for(digit_index = 0; ; index++)
	{
		char c = spec[index];
		if(c == '<')
		{	
			//Set a char array which contains either x or y,
			//depending if the char that succeeds '<' is a 
			//'r' for 'row' or a 'c' for 'column'
			char *number_string;
			if((c = spec[++index]) == 'r')
				number_string = int_to_char(x);
			else if(c == 'c')
				number_string = int_to_char(y);
			//Go back to where the angle bracket was
			--index;
			//Until we reach the other angle bracket, insert the digit string
			//into title without incrementing index 
			while(spec[index + digit_index] != '>' && digit_index < strlen(number_string))
			title[index + digit_index] = number_string[digit_index++];					
		
			(c = spec[++index]);
		}
		else if(c  == 'c')
			break;
		else
			title[index + digit_index] = c;	
	}
		
    return (const char *) title;
} 

void writeTile(PgmImage source, const char *origin_file_name, const char *target_file_name, unsigned int factor, int *return_value)
{
    unsigned int factor_squared = factor * factor;
    PgmImage tiles[factor_squared];
	unsigned int source_size = source.width * source.height;
	
	int origin_x, origin_y;
	origin_x = 0;
	origin_y = 0;

    for(int i = 0; i < factor_squared; ++i)
    {
		tiles[i] = copyPgmMetadata(source); 
		tiles[i].filename = createFileName(target_file_name, 
		i / factor, i % factor, source.magicNumber[1]);		
		tiles[i].width = (source.width / factor);
        	tiles[i].height = (source.height / factor);
		if((*return_value = reMallocData(&tiles[i])) != 0) 
		{
			printOutMsg(FAILED_MALLOC, "./pgmTile", i
			tiles[i].filename, "");
		}
		if(i / factor == (factor - 1))
			tiles[i].width += (source.width % factor);
		if(i % factor == (factor - 1))
			tiles[i].height += (source.height % factor);
	}

	int tileCounter = 0;
	for (int currOriginRowStart = 0; 
	currOriginRowStart < source.width; 
	currOriginRowStart += (source.width / factor))
	{
		for (int currOriginColStart = 0; 
		currOriginColStart < source.height; 
		currOriginColStart += (source.height / factor))
		{
			
			for(int currOriginRow = currOriginRowStart; 
			currOriginRow < (currOriginRowStart + 
			tiles[tileCounter].width ); currOriginRow++)
				for(int currOriginCol = currOriginColStart; 
				currOriginCol < (currOriginColStart + 
				tiles[tileCounter].height); currOriginCol++) 
				{
					tiles[tileCounter].imageData[currOriginRow - currOriginRowStart][currOriginCol - currOriginColStart] = source.imageData[currOriginRow][currOriginCol];
				}
				
			++tileCounter;
		}
	}
 


	 for(int i = 0; i < factor_squared; ++i)
    {
		pgmWrite(tiles[i].filename, tiles[i], return_value);
	}
}

int main(int argc, char ** argv)
{

	int return_val = 0;
	//Return a usage error if we have the wrong number of values
	if(argc == 1)
        return printOutMsg(USAGE_ERROR, argv[0], "", "");
    else if(argc != 4)	
		return printOutMsg(BAD_ARG_NO, argv[0], "", "");

	//In case our tile factor has more than one digit
	//we want to make it so we read every digit in properly
	//instead of just reading the first digit we read
	//in multi-digit numbers properly
	unsigned int my_factor, counter, numlen;
	my_factor = counter = numlen = 0;
	//Count the number of chars in argv[2]; the reduce factor
	while(argv[2][++numlen]);
	//While we count up through the addresses in argv[2],
	while(counter <= numlen)
	{
	//declare a char that represents the character representing the digit
		char c = (argv[2][counter++]);
	//then create an uint to represent the actual digit
	        unsigned int digit = c - '0';
	//shift the digit in decimal to the appropriate place
		for(int index = 1; index < numlen; index++)
		{
			digit *= 10;
		}
	//then add it to the factor
		my_factor += digit;
		numlen--;
	}

    writeTile(pgmRead(argv[1], &return_val), 
	argv[1], argv[3],   my_factor, &return_val);
	return printOutMsg(return_val , argv[0], argv[3], "");
}
