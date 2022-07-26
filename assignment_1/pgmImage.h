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
	unsigned char *imageData;
	const char *filename;
	char **comments;
	unsigned int numComments;
} PgmImage;

PgmImage createDefaultPgmObject();
PgmImage createBinaryNullDataPgm();
PgmImage createASCIINullDataPgm();
void assignBasePgmValues(PgmImage *a);
void printComments(PgmImage *this);
void printImage(PgmImage *this);
PgmImage copyPgm(PgmImage ref);
#endif //__PGM_IMAGE__
