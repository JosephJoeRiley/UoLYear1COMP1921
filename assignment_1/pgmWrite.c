#include "pgmImage.h"

void writeContentsBinary(PgmImage input, FILE *input_file, const char *filename)
{
	long dataStart = ftell(input_file);
	long dataLength = input.width * input.height * sizeof(unsigned char);
	fclose(input_file);
	FILE *file_to_write = fopen(filename, "ab");
	fseek(file_to_write, dataStart, SEEK_SET);
	fwrite(input.imageData, sizeof(unsigned char), dataLength, file_to_write);
}
void writeContentsASCII(PgmImage input, FILE *file_to_write)
{
	//Loop through every pixel in the file: if we have a new line
	//then print an additional new line	
	//This is based on my code from pgmRead, which I wrote first
	//However, the body is largely changed and cased on pgmEchoCommented
	unsigned int dataLength = input.width * input.height * sizeof(unsigned char);
	for(unsigned char *nextGray = input.imageData; 
	nextGray < (input.imageData + dataLength); nextGray++)
	{
		//get the position we are in and see if that position is 
		//divisible by the width value
		int colSplit = (nextGray - input.imageData + 1) % input.width;
		 fprintf(file_to_write, "%d%c", *nextGray, (colSplit? ' ' : '\n'));
	}
}
/*	PGMWRITE OUTLINE
Check pgmObject_a's contents are valid
		Open file_a
		Write pgmObject_a's contents to file_a
		Close file_a
		Output WRITTEN error message
*/
void pgmWrite(char *filename, PgmImage input, int *return_value)
{
	FILE *file_to_write = fopen(filename, "w");
	
	if(file_to_write == NULL)
	{
		printOutMsg(MISC_ERROR, "./pgmWrite", filename, "Writing to non-existent directory");
	}

	//Print the metadata to the output file
	size_t nBytesWritten = fprintf(file_to_write, "%c%c\n%d %d\n%d\n", input.magicNumber[0], input.magicNumber[1], 
	input.width, input.height, input.maxGray);

	//We've failed in outputting if this metadata input fails
	if(nBytesWritten < 0)
	{
		*return_value = FAILED_OUTPUT;
	}

	printf("Writing contents to a");
	switch (input.magicNumber[1])
	{
	case '2':
		printf("n ASCII pgm (%s)\n", filename);
		writeContentsASCII(input, file_to_write);
		fclose(file_to_write);
		break;
	case '5':
		printf(" binary pgm (%s)\n", filename);
		writeContentsBinary(input, file_to_write, filename);
		//No need to close the file since writeContentsBinary already closes the file
		break;
	}
	
	if(nBytesWritten < 0)
	{
		*return_value = FAILED_OUTPUT;
	}

	printOutMsg(*return_value,"./pgmWrite", filename, "");
	return;

}
