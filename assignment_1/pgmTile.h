#ifndef __PGM_TILE__
#define __PGM_TILE__
#define TILED 0
#include <math.h>
#include "pgmImage.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"
char *int_to_char(unsigned int input);
const char *createFileName(const char *spec, unsigned int x, unsigned int y);
void writeTile(PgmImage source, const char *origin_file_name, 
const char *target_file_name, unsigned int factor, int *return_value);
#endif //__PGM_TITLE__