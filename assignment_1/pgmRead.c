#include "pgmRead.h"


//This is basically a destructor 
//it frees our comments
void freeComments(PgmImage this)
{
	for(int i = this.numComments-1; i>=0; --i) 
	{
		free(this.comments[i]);
	}
}

//A collection of string.h functions that
//skips the line and writes it into a comment 
//string that is handled in the caller function
void writeComment(char **comment, FILE *file, int *error_return)
{
	char *buffer = (char *) malloc(MAX_COMMENT_LINE_LENGTH * sizeof(char));
	if(buffer == NULL) 
	{
		char c;
		while ((c = fgetc(file)) != '\n')
		;
		*error_return = BAD_COMMENT;
		return; 
	}
	fgets(buffer, MAX_COMMENT_LINE_LENGTH, file);
	*comment = (char *) malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(*comment, buffer);
}

//Wrapper of fgetc() to check if we land on the start of a comment
char pgmCharWrapper(char c, FILE *file, char **comment, unsigned int *count, int *err)
{
	if(c == '#') {
		++*count;
		writeComment(comment, file, err);
		c = fgetc(file);
	}	
	return c;
}

//Wrapper for fscanf to check if we land on a comment
//This returns a null value if it finds a comment 
//and a success value if it doesn't find one:
//giving the file pointer the green light to continue scanning
int pgmScanWrapper(int input, FILE *file, char **comment, unsigned int *count, int *err)
{
	if(fgetc(file) == '#') 
	{
		++*count;
		writeComment(comment, file, err);
		return 0;
	}
	return input;
}

/*
	pgmRead
	Outline of pgmRead.c(pointer_to_filename_a, pointer_to_pgmObject_a)
		READ
		Make pgmObject_a default (in case there is already data)
		Open file_a
		Read contents of file_a into pgmObject_a
		Close file_a
		Output READ error message
		
	DATAWRAPPER
		See if the given data is the beginning of a comment
		if so, write the comment and return failure
		if not return the initial data
*/

//Returns the image data of the file in a binary format 
void getBinaryContents(int *err_val, PgmImage *target_pgm, FILE *input_file)
{
	//We're using the pointer of the file so we can adjust it
	long dataStart = ftell(input_file);
	long dataLength = target_pgm->width * target_pgm->height * sizeof(unsigned char);
	fclose(input_file);
	//Allocate the memory for reading data
	FILE *binary_file = fopen(target_pgm->filename, "rb");
	fseek(binary_file, dataStart, SEEK_SET);
	
	for (int i = 0; i < target_pgm->width; i++) 
	{
		fread(target_pgm->imageData[i], sizeof(unsigned char), target_pgm->height * sizeof(unsigned char), binary_file);
	}

	fclose(binary_file);	
}

void getASCIIContents(int *err_val, PgmImage *target, FILE *input)
{
	long dataLength = target->width * target->height * sizeof(unsigned char);
	for(int pixel_row = 0; pixel_row < target->width; ++pixel_row)
		for(int pixel_col = 0; pixel_col < target->height; ++pixel_col)
		{
			if(fscanf(input, " %u", (unsigned int *) &target->imageData[pixel_row][pixel_col]) != 1 ||
			target->imageData[pixel_row][pixel_col] > target->maxGray)
			{
				fclose(input);
				free(target->imageData);
				*err_val = BAD_DATA;
				return;
			}
		}
	fclose(input);
}

//Returns a pgmImage object that is equivalent
// to the file stored in filename, reference passed
//int to return any error codes
PgmImage pgmRead(const char *filename, int *err_value)
{
	int scanSuccess = 0;
	*err_value = 0;
	PgmImage output = createDefaultPgmObject();
	FILE *file_to_read = fopen(filename, "r");
	if(file_to_read == NULL)
	{
		*err_value = BAD_FILENAME;
		printOutMsg(*err_value, "./pgmRead", filename, "");
		return createDefaultPgmObject();
	}
    output.filename = filename;
//This next bit with the magic number accounts for endianess:
//all paths will return a value since I'm assuming it's one or the other always
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	output.magicNumber[0] = pgmCharWrapper(fgetc(file_to_read), 
	file_to_read, &(output.comments[output.numComments]), &(output.numComments), err_value);
	//Unprotected since no one would put a comment between the two magic number chars
	output.magicNumber[1] = fgetc(file_to_read);
#else
	output.magicNumber[1] = pgmCharWrapper(fgetc(file_to_read), 
	file_to_read, &(output.comments[output.numComments]), &(output.numComments), err_value);
	output.magicNumber[0] = fgetc(file_to_read);
#endif	

	//if we encounter an error, then we set the error value, destruct and then return a null pgm object
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	if(output.magicNumber[0] != 'P' || (output.magicNumber[1] != '2' && output.magicNumber[1] != '5'))
#else
	if(output.magicNumber[1] != 'P' || (output.magicNumber[0] != '2' && output.magicNumber[1] != '5'))
#endif	
	{
		*err_value = BAD_MAGIC_NO;
		fclose(file_to_read);
		return createDefaultPgmObject();
	}

	if(*err_value)
	{
		printOutMsg(*err_value, "./pgmRead", filename, "");
		freeComments(output);
		return createDefaultPgmObject();
	}
	//in case we have multiple comments before the dimensions are stated
	//we run the comment check however many times we need to
	do
	{
		scanSuccess = pgmScanWrapper(fscanf(file_to_read, " %u %u", &(output.width), 
		&(output.height)), file_to_read, &(output.comments[output.numComments]), &(output.numComments), err_value);
	} while (!scanSuccess);

   if(*err_value)
	{
		printOutMsg(*err_value, "./pgmRead", filename, "");
		freeComments(output);
		return createDefaultPgmObject();
	}

	if( scanSuccess != 2 ||
	(output.height < MIN_IMAGE_DIMENSION || output.height > MAX_IMAGE_DIMENSION) 
	||
	(output.width < MIN_IMAGE_DIMENSION || output.width > MAX_IMAGE_DIMENSION))
	{
		*err_value = BAD_DIMENSIONS;
		printOutMsg(*err_value, "./pgmRead", filename, "");
		freeComments(output);
		return createDefaultPgmObject();
	}

	
	//same with dimensions, we may have multiple comment lines so best be safe
	do 
	{	
		scanSuccess = pgmScanWrapper(fscanf(file_to_read, " %u", &(output.maxGray)), 
		file_to_read, &(output.comments[output.numComments]), &(output.numComments), err_value);
	} while(!scanSuccess);

	if(*err_value)
	{
		printOutMsg(*err_value, "./pgmRead", filename, "");
		freeComments(output);
		return createDefaultPgmObject();
	}

	if (scanSuccess != 1 ||
		(output.maxGray < 1 || output.maxGray > DEFAULT_MAX_GRAY))
	{
		*err_value = BAD_MAX_GRAY;
		printOutMsg(*err_value, "./pgmRead", filename, "");
		freeComments(output);
		return createDefaultPgmObject();
	} 

	*err_value = reMallocData(&output);
	if(*err_value)
	{
		printOutMsg(*err_value, "./pgmRead", filename, "");
		freeComments(output);
		return createDefaultPgmObject();
	}
	//This is a switch statement to make it easier to read
	switch (output.magicNumber[1])
	{
	case '5':
		getBinaryContents(err_value, &output, file_to_read);
		break;
	case '2':
		getASCIIContents(err_value, &output, file_to_read);
		break;
	}

	printOutMsg(*err_value, "./pgmRead", filename, "");
	freeComments(output);
	return output;
}

