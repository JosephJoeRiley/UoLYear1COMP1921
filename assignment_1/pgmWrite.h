#ifndef __PGM_WRITE__
#define __PGM_WRITE__
#include "pgmImage.h"
#include <stdio.h>
void writeContentsBinary(PgmImage input, FILE *input_file, const char *filename);
void writeContentsASCII(PgmImage input, FILE *file_to_write);
void pgmWrite(const char *file_to_write, PgmImage input, int *return_value);
#endif //__PGM_WRITE__