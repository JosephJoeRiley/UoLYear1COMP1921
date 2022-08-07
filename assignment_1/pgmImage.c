#include "pgmImage.h"



#include "pgmImage.h"


//Creates a null pgmImage with no values
//meant to trip errors
//if we want to create anything else
//we read in a pgm file or create it member
//by member in the relevant modulue
PgmImage createDefaultPgmObject(void)
{
	PgmImage a;
	a.magicNumber[0] = 'P';
	a.magicNumber[1] = '0';
	a.width = 0;
	a.height = 0;
	a.maxGray = DEFAULT_MAX_GRAY;
	a.imageData = (unsigned char **) "";
	a.numComments = 0;
	a.comments = (char**) malloc(MAX_COMMENT_AMOUNT * sizeof(char*));
	a.filename = "Null.pgm";
	return a;
}

PgmImage copyPgmMetadata(PgmImage b)
{
	PgmImage a;
	a.magicNumber[0] = (char) b.magicNumber[0];
	a.magicNumber[1] = (char) b.magicNumber[1];
	a.width = b.width;
	a.height = b.height;
	a.maxGray = b.maxGray;
	a.imageData = (unsigned char **) "";
	a.numComments = b.numComments;
	a.comments = b.comments;
	a.filename = b.filename;
	return a;
}
void printInConsole(PgmImage ref) 
{
	printf("Data from pgm:\nMagicNumber: %s, Dimensions: %dx%d, MaxGray= %d\n\nImage data= \n%s\n", 
	ref.magicNumber, ref.width, ref.height, ref.maxGray, (char *) ref.imageData);
}

//Procedure for debugging purposes
//to check if comments have been 
//stored correctly
void printComments(PgmImage *this)
{
	for(int i=0; i<this->numComments; ++i) {
		printf("Data in comments[%d] = %s\n", i, this->comments[i]);
	}
}

int reMallocData(PgmImage *this)
{
	this->imageData = (unsigned char**) malloc(this->width * sizeof(unsigned char *));
	for(int i = 0; i <= this->width; i++)
	{
		this->imageData[i] = (unsigned char *) malloc(this->height * sizeof(unsigned char));
		if(this->imageData[i] == NULL) 
		{
			free(this->imageData[i]);
			return FAILED_MALLOC;
		}
	}
	if(this->imageData == NULL)
	{
		free(this->imageData);
		return FAILED_MALLOC;
	}
	else 
		return 0;
}
