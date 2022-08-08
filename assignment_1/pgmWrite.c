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
	//We want to write <width> chars per line of data
	long line_size = input->width * sizeof(unsigned char);
	//We want to write <height> amount of <width> length char arrays
	for (int i = 0; i < input->height; i++)
	{
		if(fwrite(input->imageData[i], sizeof(unsigned char), 
		line_size, binary_file) != line_size) {
			*err_val = FAILED_OUTPUT;
			return;
		}
	}
	fclose(binary_file);	
}
void writeContentsASCII(int *err_val, PgmImage input, FILE *file_to_write)
{
	//Loop through every pixel in the file: if we have a new line
	//then print an additional new line
    *err_val = 0;
	//This is the amount of characters we've written as a whole 
	int charCount= 0;	
	//This is the amount of integers/pixels that have been written in 
	int pixelsWritten = 0;
	for(int pixel_col = 0; pixel_col < input.height; ++pixel_col) {
		for(int pixel_row = 0; pixel_row < input.width; ++pixel_row)
		{
			unsigned char grayValue = input.imageData[pixel_col][pixel_row];
			if(grayValue > input.maxGray && grayValue < 0)
			{
				printf("Bad pixel\n");
				*err_val = 8;
				break;
			}
			fprintf(file_to_write, "%d", grayValue);
			if(pixel_row == input.width)
			{
				if(pixel_col != input.height)
				{
					fprintf(file_to_write, "\n");
				}
			}
			else
			{
				fprintf(file_to_write, " ");
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
