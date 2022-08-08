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
//Has the error code as an arg so we can throw bad_comment
void writeComment(char **comment, int comment_index,  FILE *file, int *error_return)
{
	char *buffer = (char *) malloc(MAX_COMMENT_LINE_LENGTH * sizeof(char));
	//If our malloc of the character array fails then
	//we say the comment is bad and skip to the next line
	//so that our scanf and getc wrappers are happy (i.e. returning valid image data)
	if(buffer == NULL) 
	{
		char c;
		while ((c = fgetc(file)) != '\n')
		;
		*error_return = BAD_COMMENT;
		return; 
	}
	fgets(buffer, MAX_COMMENT_LINE_LENGTH, file);
	if(buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == EOF)
	{
		comment[comment_index] = (char *) malloc((strlen(buffer)) * sizeof(char));
		strcpy(comment[comment_index], buffer);
		return;
	}
	//This clause triggers if we haven't reached a newline or the end of file
	//by our max line length, which means the comment is too long
	else 
	{
		*error_return = BAD_COMMENT;
		return; 
	}
}

//Wrapper of fgetc() to check if we land on the start of a comment
//Calls writeComment if we do (we need the image as an arg so we can get its comment data)
char pgmCharWrapper(char c, FILE *file, PgmImage *image, int *err)
{
	if(c == '#') {
		//Add an additional comment to the image
		++image->numComments;
		writeComment(image->comments, image->numComments - 1,  file, err);
		c = fgetc(file);
	}	
	return c;
}

//Wrapper for fscanf to check if we land on a comment
//and returns the number of values read in in any case:
//giving the file pointer the green light to continue scanning
//Calls writeComment if we do (we need the image as an arg so we can get its comment data)
int pgmScanWrapper(int input, FILE *file, PgmImage *image, int *err)
{
	if(fgetc(file) == '#') 
	{
		//The image now has one more comment
		++image->numComments;
		writeComment(image->comments, image->numComments - 1, file, err);
		//We didn't read anything in this time aound
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

//Writes in the image data of a binary file to target_pgm 
void getBinaryContents(int *err_val, PgmImage *target_pgm, FILE *input_file)
{
	//We're using the pointer of the file so we can read from that point onward
	long dataStart = ftell(input_file);
	//We're done with the argument file since it was in ASCII format for the metadata
	fclose(input_file);
	//Reopen and get back to where we were (the image contents)
	FILE *binary_file = fopen(target_pgm->filename, "rb");
	fseek(binary_file, dataStart, SEEK_SET);
	
	//Read in the image data line by line, 
	for (int i = 0; i < target_pgm->width; i++) 
	{
		fread(target_pgm->imageData[i], sizeof(unsigned char), 
		(target_pgm->height * sizeof(unsigned char)), binary_file);
	}

	//Binary file won't exist for the main function
	//since we had to reopen it, so we're closing here
	fclose(binary_file);	
}

//Writes in the contents of an ASCII file to target(a PGM object)
void getASCIIContents(int *err_val, PgmImage *target, FILE *input)
{
	for(int pixel_row = 0; pixel_row < target->width; ++pixel_row)
	{
		for(int pixel_col = 0; pixel_col < target->height; ++pixel_col)
		{
			int scanCount = 0;

			//First check if our data is valid and we've managed to scan exactly one integer in
			if((scanCount = pgmScanWrapper(fscanf(input, " %u", 
			(unsigned int *) &target->imageData[pixel_row][pixel_col]), input, target, err_val)) != 1)
			{
				//pgmScanWrapper will have written
				//any comments for us.
				//In case any of these comments 
				//are too long, we return bad comment
				if(*err_val == BAD_COMMENT)
					return;
				//Check if our pixel is is valid
				else if (target->imageData[pixel_row][pixel_col] > target->maxGray || 
				target->imageData[pixel_row][pixel_col] < 0) 
				{
					//A pixel is out of the acceptable range
					*err_val = BAD_DATA;
					return;
				}
				else if(feof(input))  
				{
					//We reached the end of the file (not enough data)
					*err_val = BAD_DATA;
					return;
				}
				//None of the above have occurred, so the data is valid
				//We go to the top of the loop for the next pixel
			}
		}
	}
	
	//Final check if this is actually the end 
	//of the file
	unsigned int endCheck;
	
	//If this scan comes back having read something
	//then we aren't at the end of the data: so
	// we return BAD_DATA for too much data
	if(fscanf(input, " %u", &endCheck) != 0)
		*err_val = 8;
	
	//This is to keep parity with our binary reading
	//function, to avoid closing the image twice and 
	//causing a double free
	fclose(input);
}

//Returns a pgmImage object that is equivalent
// to the file stored in filename, reference passed
//int to return any error codes
PgmImage pgmRead(const char *filename, int *err_value)
{

	int scanSuccess = 0;
	//Make sure this is initialised so we don't get 
	//any unpredictable returns
	*err_value = 0;
	PgmImage output = createDefaultPgmObject();
	FILE *file_to_read = fopen(filename, "r");
	if(file_to_read == NULL)
	{
		*err_value = BAD_FILENAME;
		//We're writing a string literal as if this were 
		//an executable because the error return module 
		//checks for a string in this format
		printOutMsg(*err_value, "./pgmRead", filename, "");
		return createDefaultPgmObject();
	}
    output.filename = filename;
//This next bit with the magic number accounts for endianess:
//all paths will return a value since I'm assuming it's one or the other always
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	output.magicNumber[0] = pgmCharWrapper(fgetc(file_to_read), 
	file_to_read, &output, err_value);
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
		if(output.comments)
			freeComments(output);
		return createDefaultPgmObject();
	}

	//in case we have multiple comments before the dimensions are stated
	//we run the comment check however many times we need to
	do
	{
		scanSuccess = pgmScanWrapper(fscanf(file_to_read, " %u %u", &(output.width), 
		&(output.height)), file_to_read, &output, err_value);
	} while (!scanSuccess);

	//These lines of code appear muliple times, mostly in case we get a bad comment
	//that would slip through the cracks and mess with the image data down the line
	//otherwise
   if(*err_value)
	{
		if(output.comments)
			freeComments(output);
		return createDefaultPgmObject();
	}

	if( scanSuccess != 2 ||
	(output.height < MIN_IMAGE_DIMENSION || output.height >= MAX_IMAGE_DIMENSION) 
	||
	(output.width < MIN_IMAGE_DIMENSION || output.width >= MAX_IMAGE_DIMENSION))
	{
		*err_value = BAD_DIMENSIONS;
		if(output.comments)
			freeComments(output);
		return createDefaultPgmObject();
	}

	
	//same with dimensions and the max gray, we may have multiple comment lines so best be safe
	do 
	{	
		scanSuccess = pgmScanWrapper(fscanf(file_to_read, " %u", &(output.maxGray)), 
		file_to_read, &output, err_value);
	} while(!scanSuccess);

	if(*err_value)
	{
		if(output.comments)
			freeComments(output);
		return createDefaultPgmObject();
	}

	if (scanSuccess != 1 ||
		(output.maxGray < 1 || output.maxGray > DEFAULT_MAX_GRAY))
	{
		*err_value = BAD_MAX_GRAY;
		if(output.comments)
			freeComments(output);
		return createDefaultPgmObject();
	} 

	*err_value = reMallocData(&output);
	if(*err_value)
	{
		if(output.comments)
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

	//In case of a bad comment or bad data
	if(*err_value)
	{
		if(output.comments)
			freeComments(output);
		return createDefaultPgmObject();
	}

	//Return success since we would have
	//returned if there were any problems (hopefullly)
	if(output.comments)
			freeComments(output);
	return output;
}

