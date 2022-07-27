#include "pgmTile.h"


const char *createFileName(const char *spec, unsigned int x, unsigned int y, char mID)
{

	char *title = (char *) malloc(MAX_COMMENT_LINE_LENGTH * sizeof(char));
	snprintf(title, MAX_COMMENT_LINE_LENGTH, "%s_%d_%d.%s",  spec, x, y, (mID == '2')?
	"ascii.pgm" : "pgm");
    return (const char *) title;
} 

void writeTile(PgmImage source, const char *origin_file_name, unsigned int factor, int *return_value)
{
    unsigned int factor_squared = factor * factor;
    PgmImage tiles[factor_squared];
	unsigned int source_size = source.width * source.height;

    for(int i = 0; i < (factor_squared); ++i)
    {
        tiles[i] = copyPgm(source); 
        tiles[i].width = (source.width / factor);
        tiles[i].height = (source.height / factor);
        if((i/ factor) != 0)
            tiles[i].width += source.width % factor;
        if((i % factor) != 0)
            tiles[i].height += (source.height % factor);
		unsigned int tile_size = tiles[i].width * tiles[i].height;
		tiles[i].imageData =  (unsigned char *) malloc(tile_size * sizeof(unsigned char *));
        if(tiles[i].imageData == NULL)
            printOutMsg(FAILED_MALLOC, "./pgmTile", origin_file_name, "");
    }


	//Trying to  find a way so we cycle through every pixel in the image and copy
	//the pixel we're reading to the appropriate tile by doing a calculation on 
	//the pixel's location. Ideas;
	//	;
	for(int currPixel = 0, tile_index = 0; currPixel < source_size && tile_index < factor_squared; ++currPixel, ++tile_index)
	{
		tiles[tile_index].imageData[currPixel] = source.imageData[currPixel];
	}

	for(int i = 0; i < factor_squared; i++) {
		pgmWrite(createFileName(origin_file_name, (i/ factor), (i  % factor), 
        tiles[i].magicNumber[1]), tiles[i], return_value);
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

    writeTile(pgmRead(argv[1], &return_val), argv[3],  my_factor, &return_val);
	return printOutMsg(return_val , argv[0], argv[3], "");
}