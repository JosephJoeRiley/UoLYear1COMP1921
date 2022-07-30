#ifndef __PGM_IMAGE__
#define __PGM_IMAGE__
#define MAX_COMMENT_LINE_LENGTH 128
#define MAX_COMMENT_AMOUNT 256
#define MAX_IMAGE_DIMENSION 65536
#define MIN_IMAGE_DIMENSION 1
#define DEFAULT_MAX_GRAY 255
#include <stdlib.h>
#include "pgmErrors.h"
typedef struct pgmImage
{
	unsigned char magicNumber[2];
	unsigned int width;
	unsigned int height;
	unsigned int maxGray;
	const char *filename;
	unsigned char **imageData;
	char **comments;
	unsigned int numComments;
} PgmImage;

PgmImage createDefaultPgmObject();
PgmImage createBinaryNullDataPgm();
PgmImage createASCIINullDataPgm();
PgmImage copyPgm(PgmImage ref);
int reMallocData(PgmImage *this);
void assignBasePgmValues(PgmImage *a);
void printComments(PgmImage *this);
void printImage(PgmImage *this);
void printInConsole(PgmImage this);
#endif //__PGM_IMAGE__
