#include "pgmErrors.h"


//Usage: leave filename and/or miscMessage blank if 
//they will be unsused
int printOutMsg(int errorCode, const char *programName, 
const char *filename, const char *miscMessage) 
{
	char callerID[strlen(programName) - 4];
	
	//Create a string that chops off the leading 
	//"./pgm" so our main switch statement is more 
	//readable
	//We need the entire title, not only for 
	//readablility, since the names of 
	//the modules may share inital letters
	for(int i = 4; i < strlen(programName); ++i)
		callerID[i - 4] = programName[i];

	switch(errorCode)
	{
		case -1:
		if(callerID == "Compare")
			printf("DIFFERENT");
			errorCode = 0;
		break;
		case 0:		
			if(callerID == "Read")
				printf("READ");
			else if(callerID == "Write")
				printf("WRITTEN");
			else if(callerID == "Echo")
				printf("ECHOED");	
			else if(callerID == "Compare")
				printf("IDENTICAL");
			else if(callerID ==  "a2b" || callerID == "b2a") 
				printf("CONVERTED");
			else if(callerID == "Reduce")
				printf("REDUCED");
			else if(callerID == "Tile")
				printf("TILED");
			else if(callerID == "Assemble")
				printf("ASSEMBLED");
		break;
		
		break;
		case 1 ... 100:
			printf("ERROR: ");
		//We're using the defines here in case 
		//the error codes are changed around at any point
		switch(errorCode) 
		{
			///Wrong number of arguments (but not zero arguments)
 			case BAD_ARG_NO:
				printf("Bad Argument Count");
			break;
			//Read filename does not exist
 			case BAD_FILENAME:
				printf("Bad File Name ");
			break;
			//Magic number is invalid
 			case BAD_MAGIC_NO:
				printf("Bad Magic Number ");
			break;
			//Comment line is corrupted or otherwise incorrect
 			case BAD_COMMENT:
				printf("Bad Comment Line");
			break;
			//Width and/or height are outside the min_max range
 			case BAD_DIMENSIONS:
				printf("Bad Dimensions ");
			break;
			//Max gray does not equal 255
 			case BAD_MAX_GRAY:
				printf("Bad Max Gray Value");
			break;
			//Malloc of data for pointer failed 
 			case FAILED_MALLOC:
				printf("Image Malloc FaMiscellaneous iled");
			break;
		 	//Reading in image data failed
 			case BAD_DATA:
				printf("Bad Data");
			break;
			//Writing in image data failed
 			case FAILED_OUTPUT:
				printf("Output Failed");
			break;
			//Assembly layout went wrong
 			case BAD_LAYOUT:
				printf("Bad Layout");
			break;
			//Any other error
 			case MISC_ERROR:
				printf("Miscellaneous");
			break;
		}
		break;
		case 101:
		printf("Usage: ");
			if(callerID == "Echo")
				printf("./pgmEcho inputImage.pgm outputImage.pgm");
			if(callerID == "Comp") 
				printf("./pgmComp inputImage.pgm inputImage.pgm");
			if(callerID == "a2b")
				printf("./pgma2b inputImage.pgm outputImage.pgm");
			if(callerID == "b2a")
				printf("./pgmb2a inputImage.pgm outputImage.pgm");
			if(callerID == "Reduce")
				printf("./pgmReduce inputImage.pgm reduction_factor outputImage.pgm");
			if(callerID == "Tile")
				printf("./pgmTile inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm");
			if(callerID == "Assemble")
				printf("./pgmAssemble outputImage.pgm width height (row column inputImage.pgm)+");
		break;
	}
	//Only printing the file name
	//and/or message if they exist
	if(filename != "")
		printf(" (%s)", filename);
	if(miscMessage != "")
		printf(" (%s)", miscMessage);
	printf("\n");
	return errorCode;
}
