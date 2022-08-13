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

//When creating a copy 
//we only copy the metadata for 
//efficiency's sake:
//Pgmtile, reduce etc, will all 
//write their image data in by 
//themselves
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

//This is for debugging purposes
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

//Any time our width and/or height changes, we'll want to
//malloc our image data again,
//Now that we're using a double pointer, mallocing is a more 
//complex process that calls for it's own method anyhow
int reMallocData(PgmImage *this)
{
	//Allocate <width> amount of unsigned char arrays
	this->imageData = (unsigned char**) malloc(this->height * sizeof(unsigned char *));
	if(this->imageData == NULL)
	{
		free(this->imageData);
		return FAILED_MALLOC;
	}
    //Allocate <height> amount of  <width> length char arrays
	for(int i = 0; i <= this->height; i++)
	{
		this->imageData[i] = (unsigned char *) malloc(this->width * sizeof(unsigned char));
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
