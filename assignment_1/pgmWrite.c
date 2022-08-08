#include "pgmImage.h"


void writeContentsBinary(int *err_val, PgmImage *input, FILE *file_to_write, const char* write_filename)
{
	//Where does the image data start?
	long dataStart = ftell(file_to_write);
	//Reopen so that we can write binary
	fclose(file_to_write);
	FILE *binary_file = fopen(write_filename, "ab");
	//Go to the start of the image data
	fseek(binary_file, dataStart, SEEK_SET);
	//We want to write <height> chars per line of data
	long line_size = input->height * sizeof(unsigned char);
	//We want to write <width> amount of <height> length char arrays
	for (int i = 0; i < input->width; i++)
	{
		if(fwrite(input->imageData[i], sizeof(unsigned char), 
		line_size, binary_file) != line_size) {
			err_val = FAILED_OUTPUT;
			return;
		}
	}
	fclose(binary_file);	
}
void writeContentsASCII(int *err_val, PgmImage input, FILE *file_to_write)
{
	//Loop through every pixel in the file: if we have a new line
	//then print an additional new line

	//This is the amount of characters we've written as a whole 
	int charCount= 0;	
	//This is the amount of integers/pixels that have been written in 
	int pixelsWritten = 0;
	for(int pixel_row = 0; pixel_row < input.width; ++pixel_row) {
		for(int pixel_col = 0; pixel_col < input.height; ++pixel_col)
		{
			//We want to know how many digits are in a given pixel
			int digitCount = 0;
			int grayVal = input.imageData[pixel_row][pixel_col];
			//First write the pixel in, checking if we've succeeded
		 	if(fprintf(file_to_write, "%d", grayVal) != 1)
			{
				*err_val = FAILED_OUTPUT;
			}
			++pixelsWritten;
			//Since this isn't our actual pointer, we can chop it up
			//and count its digits
			while(grayVal /= 10)
				digitCount++;
			//We've now written <digit> amount of chars
			charCount += digitCount;
		//The pgm specification states that no line should have more than
		//70 characters, hence the first clause of the if statement
		//However, there seems to be some formula done on the dimensions/width
		//of the image from the input ASCII images I've looked at:
		//E.g.: a 512x512 image has at most 12 pixels written per column
		//A 460x360 image will have 19 at most pixels written per column
			if(charCount > 0 && charCount % 70 == 0
			|| pixelsWritten % (input.width / 42) == 0)
				fprintf(file_to_write, "\n");
			else {
				//Spacebars are also characters, so better increment charCount
				fprintf(file_to_write, " ");
				charCount += 1;
			}
		}
	}

	fclose(file_to_write);
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
	
	//This is here in case we write to a non-existent directory
	//or the filename is otherwise non-writable
	if(file_to_write == NULL)
	{
		*return_value = BAD_FILENAME;
		return;
	}

	//Print the metadata to the output file
	size_t nBytesWritten = fprintf(file_to_write, "%c%c\n%d %d\n%d\n", input.magicNumber[0], input.magicNumber[1], 
	input.width, input.height, input.maxGray);

	//We've failed in outputting if this metadata input fails
	if(nBytesWritten < 0)
	{
		*return_value = FAILED_OUTPUT;
		return;
	}
	
	//printf("Writing contents to a");
	switch (input.magicNumber[1])
	{
	case '2':
	//	printf("n ASCII pgm (%s)\n", filename);
		writeContentsASCII(return_value, input, file_to_write);
		break;
	case '5':
	//	printf(" binary pgm (%s)\n", filename);
		writeContentsBinary(return_value, &input, file_to_write, filename);
		break;
	}
	
	//printOutMsg(*return_value,"./pgmWrite", filename, "");
	return;

}
