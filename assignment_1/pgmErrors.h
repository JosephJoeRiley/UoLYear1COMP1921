#ifndef __PGM_ERRORS__
#define __PGM_ERRORS__

///Wrong number of arguments (but not zero arguments)
#define BAD_ARG_NO 1
//Read filename does not exist
#define BAD_FILENAME 2
//Magic number is invalid
#define BAD_MAGIC_NO 3
//Comment line is corrupted or otherwise incorrect
#define BAD_COMMENT 4
//Width and/or height are outside the min_max range
#define BAD_DIMENSIONS 5
//Max gray does not equal 255
#define BAD_MAX_GRAY 6
//Malloc of data for pointer failed 
#define FAILED_MALLOC 7
//Reading in image data failed
#define BAD_DATA 8 
//Writing in image data failed
#define FAILED_OUTPUT 9
//Assembly layout went wrong
#define BAD_LAYOUT 10
//Any other error
#define MISC_ERROR 100
//Zero arguments supplied
#define USAGE_ERROR 101

//Including for editing the programName string
#include <string.h>
//Included for outputting to the console
#include <stdio.h>

//Takes in the error code, returns the 
//appropriate message for the program in
//question (using argv[0] or any string of 
//the form "./pgm*" to determine what called it )
//and, optionally, the filename of the file being
//read or written to or, if the error code is not 
//already present, a message of 100 characters or less
//for the miscellaneous errors. Note: if either
//of the final two args are not needed they are called with 
//an empty string 
int printOutMsg(int errorCode, 
const char *programName, 
const char *filename,
const char *miscMessage);

#endif //__PGM_ERRORS__
