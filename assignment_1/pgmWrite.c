#include "pgmImage.h"


void writeContentsBinary(int *err_val, PgmImage *input, FILE *file_to_write, const char* write_filename)
{
	//Where does the image data start?
	//Reopen so that we can write binary
	//Go to the start of the image data
	//We want to write <width> chars per line of data
	long line_size = input->width * sizeof(unsigned char);
	//We want to write <height> amount of <width> length char arrays
	for (int i = 0; i < input->height; i++)
	{
		if(fwrite(input->imageData[i], sizeof(unsigned char), 
		line_size, file_to_write) != line_size) {
			*err_val = FAILED_OUTPUT;
			return;
		}
	}
}
void writeContentsASCII(int *err_val, PgmImage input, FILE *file_to_write)
{
	//Loop through every pixel in the file: if we have a new line
	//then print an additional new line
    *err_val = 0;

    //Write <height> amount of columns
        //Write <width> amount of rows
    
	for(int pixel_row = 0; pixel_row < input.height; ++pixel_row)
		for(int pixel_col = 0; pixel_col < input.width; ++pixel_col)
		{
			int printReturn;
			if(pixel_col == 0)
				printReturn = fprintf(file_to_write, "%d ", input.imageData[pixel_col][pixel_row]);
			else
				printReturn = fprintf(file_to_write, "%2d%c", 
				input.imageData[pixel_col][pixel_row],  
				(pixel_col == (input.width - 1)? (pixel_row == (input.height - 1))? EOF: '\n' : ' '));
			if(printReturn < 0)
			{
				*err_val = FAILED_OUTPUT;
				return;
			}
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

	fclose(file_to_write);	
	//printOutMsg(*return_value,"./pgmWrite", filename, "");
	return;

}
