#include "pgmTile.h"


const char *createFileName(const char *spec, unsigned int x, unsigned int y, char mID)
{
	
	char *title = (char *) malloc(256 * sizeof(char));
	/*
	char currentChar;
	int charIndexOutput, charIndexInput = charIndexOutput = 0;
	title = (char *) malloc(strlen(spec) * sizeof(char));
	do
	{
		if((currentChar = ));
		{

		}
	    title[charIndexOutput++] = currentChar;
	} while (currentChar != 'm');
	*/ 
	snprintf(title, 256, "../images/output/assignment_1/tile_%d_%d.ascii.pgm", x, y);
    return (const char *) title;
} 

void writeTile(PgmImage source, const char *origin_file_name, const char *target_file_name, unsigned int factor, int *return_value)
{
    unsigned int factor_squared = factor * factor;
    PgmImage tiles[factor_squared];
	unsigned int source_size = source.width * source.height;
	int origin_x, origin_y;
	origin_y = origin_x = 0;

    for(int i = 0; i < (factor_squared); ++i)
    {
        tiles[i] = copyPgm(source); 
        tiles[i].filename = createFileName(target_file_name, i/factor, i%factor, source.magicNumber[1]);
		
        tiles[i].width = (source.width / factor);
        tiles[i].height = (source.height / factor);
        if((i/ factor) == (factor - 1))
            tiles[i].height += source.height % factor;
        if((i % factor) == (factor - 1))
            tiles[i].width += (source.width % factor);
		
		if((*return_value = reMallocData(&tiles[i])) != 0) 
		{
			printOutMsg(FAILED_MALLOC, "./pgmTile", tiles[i].filename, "");
		}

		for(int x_coord = 0; x_coord < tiles[i].width; x_coord++, origin_x++)
		{
			if(origin_y > 0)
			{
				if(source.height % origin_y == 0)
					origin_y = 0;
			}
			for(int y_coord = 0; y_coord < tiles[i].height; y_coord++)
			{
				tiles[i].imageData[x_coord][y_coord] = source.imageData[origin_x][origin_y++];
			}
		}
	
		pgmWrite(tiles[i].filename, tiles[i], return_value);
	}
	
}

int main(int argc, char ** argv)
{

	int return_val = 0;
	//Return a usage error if we have the wrong number of values
	if(argc == 0)
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

    writeTile(pgmRead(argv[1], &return_val), argv[1], argv[3],   my_factor, &return_val);
	return printOutMsg(return_val , argv[0], argv[3], "");
}