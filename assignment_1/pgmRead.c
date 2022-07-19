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
void writeComment(char **comment, FILE *file)
{

	char *buffer = (char *) malloc(MAX_COMMENT_LINE_LENGTH * sizeof(char));
	fgets(buffer, MAX_COMMENT_LINE_LENGTH, file);
	*comment = (char *) malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(*comment, buffer);
}

//Wrapper of fgetc() to check if we land on the start of a comment
char pgmCharWrapper(char c, FILE *file, char **comment, unsigned int *count)
{
	if(c == '#') {
		++*count;
		writeComment(comment, file);
		c = fgetc(file);
	}	
	return c;
}

//Wrapper for fscanf to check if we land on a comment
//This returns a null value if it finds a comment 
//and a success value if it doesn't find one:
//giving the file pointer the green light to continue scanning
int pgmScanWrapper(int input, FILE *file, char **comment, unsigned int *count)
{
	if(fgetc(file) == '#') 
	{
		++*count;
		writeComment(comment, file);
		return 0;
	}
	return 1;
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
void getBinaryContents(int *error_return, PgmImage *target_pgm, FILE *input_file, const char *filename)
{
	//We're using the pointer of the file so we can adjust it
	long dataStart = ftell(input_file);
	long dataLength = target_pgm->width * target_pgm->height * sizeof(unsigned char);
	fclose(input_file);
	FILE *file_to_read = fopen(filename, "rb");
	//Put our file pointer so we skip the metadata
	fseek(file_to_read, dataStart, SEEK_SET);
	//Allocate the memory for reading data
	target_pgm->imageData = (unsigned char *) malloc(dataLength);
	if(target_pgm->imageData == NULL)
	{
		*error_return = FAILED_MALLOC;
	}
	fread(target_pgm->imageData, sizeof(unsigned char*),dataLength, file_to_read);
	*error_return = 0;
}

void getASCIIContents(int *error_return, PgmImage *target, FILE *input)
{
	long dataLength = target->width * target->height * sizeof(unsigned char);
	target->imageData = (unsigned char *) malloc(dataLength);

	if(target->imageData == NULL)
	{
		*error_return = FAILED_MALLOC;
	}

	for(unsigned char *nextGray = target->imageData; 
	nextGray < (target->imageData + dataLength); nextGray++)
	{
		unsigned int grayValue = 0;
		
		if(fscanf(input, " %u", &grayValue) != 1 ||
		grayValue > target->maxGray)
		{
			fclose(input);
			free(target->imageData);
			*error_return = BAD_DATA;
		}

		*nextGray = (unsigned char) grayValue;
	}

	*error_return = 0;
}

//Returns a pgmImage object that is equivalent
// to the file stored in filename, reference passed
//int to return any error codes
PgmImage pgmRead(const char *filename, int *err_value)
{
	int scanSuccess = 0;
	PgmImage output = createDefaultPgmObject();
	FILE *file_to_read = fopen(filename, "r");
	if(file_to_read == NULL)
		{
			*err_value = BAD_FILENAME;
			fclose(file_to_read);
			return createDefaultPgmObject();
		}
    output.filename = filename;
//This next bit with the magic number accounts for endianess:
//all paths will return a value since I'm assuming it's one or the other always
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	output.magicNumber[0] = pgmCharWrapper(fgetc(file_to_read), 
	file_to_read, &(output.comments[output.numComments]), &(output.numComments));
	//Unprotected since no one would put a comment between the two magic number chars
	output.magicNumber[1] = fgetc(file_to_read);
#else
	output.magicNumber[1] = pgmCharWrapper(fgetc(file_to_read), 
	file_to_read, &(output.comments[output.numComments]), &(output.numComments));
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
		//it would be a pain to put this in the handle error function
		//since we're handling what the program returns 
		return createDefaultPgmObject();
	}

	//in case we have multiple comments before the dimensions are stated
	//we run the comment check however many times we need to
	do
	{
		scanSuccess = pgmScanWrapper(fscanf(file_to_read, " %u %u", &(output.width), 
		&(output.height)), file_to_read, &(output.comments[output.numComments]), &(output.numComments));
	} while (!scanSuccess);


	//demorgan's-ing this since they are ranges after all 
	if( !(output.height >= MIN_IMAGE_DIMENSION && output.height <= MAX_IMAGE_DIMENSION) &&
	!(output.width >= MIN_IMAGE_DIMENSION && output.width <= MAX_IMAGE_DIMENSION))
	{
		*err_value = BAD_DIMENSIONS;
		freeComments(output);
		return createDefaultPgmObject();
	}

	
	//same with dimensions, we may have multiple comment lines so best be safe
	do 
	{	
		scanSuccess = pgmScanWrapper(fscanf(file_to_read, " %u", &(output.maxGray)), 
		file_to_read, &(output.comments[output.numComments]), &(output.numComments));
	} while(!scanSuccess);
	

	//demorgan's-ing this for the same reasons as dimensions 
	if (!(output.maxGray >= 1 && output.maxGray <= 255))
	{
		*err_value = BAD_MAX_GRAY;
		freeComments(output);
		return createDefaultPgmObject();
	} 

	//This is a switch statement to make it easier to read
	switch (output.magicNumber[1])
	{
	case '5':
		getBinaryContents(err_value, &output, file_to_read, filename);
		break;
	case '2':
		getASCIIContents(err_value, &output, file_to_read);
		fclose(file_to_read);
		if(*err_value)
			return createDefaultPgmObject();
		break;
	}

	
	printOutMsg(0, "./pgmRead", filename, "");
	return output;
}

